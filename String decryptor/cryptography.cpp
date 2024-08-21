
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evperr.h>
#include <openssl/crypto.h>
#include <string.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <string>
#include <iterator>
#include <fstream>


auto EncodeBase64(const std::string& to_encode) -> std::string {


	const auto predicted_len = 4 * ((to_encode.length() + 2) / 3);  

	const auto output_buffer{ std::make_unique<char[]>(predicted_len + 1) };

	const std::vector<unsigned char> vec_chars{ to_encode.begin(), to_encode.end() };  

	const auto output_len = EVP_EncodeBlock(reinterpret_cast<unsigned char*>(output_buffer.get()), vec_chars.data(), static_cast<int>(vec_chars.size()));

	if (predicted_len != static_cast<unsigned long>(output_len)) {
		throw std::runtime_error("EncodeBase64 error");
	}

	return output_buffer.get();
}

auto DecodeBase64(const std::string& to_decode) -> std::string {

	const auto predicted_len = 3 * to_decode.length() / 4;  

	const auto output_buffer{ std::make_unique<char[]>(predicted_len + 1) };

	const std::vector<unsigned char> vec_chars{ to_decode.begin(), to_decode.end() };  
	
	const auto output_len = EVP_DecodeBlock(reinterpret_cast<unsigned char*>(output_buffer.get()), vec_chars.data(), static_cast<int>(vec_chars.size()));

	if (predicted_len != static_cast<unsigned long>(output_len)) {
		throw std::runtime_error("DecodeBase64 error");
	}

	return output_buffer.get();
}


const auto enckey = DecodeBase64("a0RlZWR2MEVQYkN6aGk3aFZnYTVpTVNTVDExY0wyRXNWV01sUnFJdWxTbHlTUFhLOTdMYlQzemNsa2kyV1o4Q3ZwUFY2c3JpbE5RdXFyOEVnR29LNWM0ckNvcG9HZ3dmRkxVSmhlTG1NcjdocXI2Z1h0OHdvNU1TNXpqN0ZQTEx4OFp5OVVFd2tZaWhzNDhHTm9uTVVNbjNzYzRqRGpoSUpua1h3eG9IeEFwNkZRQk9xTFg0elFCMHBJMkRJU1Bzc2FuUG9tQURCMHZXOTF4UkpsVUJkNmR1bXA2OXNYOUxZRzZtRUdEYkdLTnpURFRGaGxJVjVtcUV1U2JzZ0pYVVU1cHN0NUpCQ2FEUjYxRHlEYTY5VHFZT2lLME1Ld2N0d0dVd0x4TkxCVWFB");

#define DECL_OPENSSL_PTR(tname, free_func) \
    struct openssl_##tname##_dtor {            \
        void operator()(tname* v) {        \
            free_func(v);              \
        }                              \
    };                                 \
    typedef std::unique_ptr<tname, openssl_##tname##_dtor> tname##_t


DECL_OPENSSL_PTR(EVP_CIPHER_CTX, ::EVP_CIPHER_CTX_free);

struct error : public std::exception {
private:
    std::string m_msg;

public:
    error(const std::string& message)
        : m_msg(message) {
    }

    error(const char* msg)
        : m_msg(msg, msg + strlen(msg)) {
    }

    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }
};

struct openssl_error : public virtual error {
private:
    int m_code = -1;
    std::string m_msg;

public:
    openssl_error(int code, const std::string& message)
        : error(message),
        m_code(code) {
        std::stringstream ss;
        ss << "[" << m_code << "]: " << message;
        m_msg = ss.str();

    }

    openssl_error(int code, const char* msg)
        : error(msg),
        m_code(code) {
        std::stringstream ss;
        ss << "[" << m_code << "]: " << msg;
        m_msg = ss.str();
    }

    const char* what() const noexcept override {
        return m_msg.c_str();
    }
};

static void throw_if_error(int res = 1, const char* file = nullptr, uint64_t line = 0) {

    unsigned long errc = ERR_get_error();
    if (res <= 0 || errc != 0) {
        if (errc == 0) {
            return;
        }
        std::vector<std::string> errors;
        while (errc != 0) {
            std::vector<uint8_t> buf(256);
            ERR_error_string(errc, (char*)buf.data());
            errors.push_back(std::string(buf.begin(), buf.end()));
            errc = ERR_get_error();
        }

        std::stringstream ss;
        ss << "\n";
        for (auto&& err : errors) {
            if (file != nullptr) {
                ss << file << ":" << (line - 1) << " ";
            }
            ss << err << "\n";
        }
        const std::string err_all = ss.str();
        throw openssl_error(errc, err_all);
    }
}

class aes256_cbc {
private:
    std::vector<uint8_t> m_iv;

public:
    explicit aes256_cbc(std::vector<uint8_t> iv)
        : m_iv(std::move(iv)) {
    }

    void encrypt(const std::vector<uint8_t>& key, const std::vector<uint8_t>& message, std::vector<uint8_t>& output) const {
        output.resize(message.size() * AES_BLOCK_SIZE);
        int inlen = message.size();
        int outlen = 0;
        size_t total_out = 0;

        EVP_CIPHER_CTX_t ctx(EVP_CIPHER_CTX_new());
        throw_if_error(1, __FILE__, __LINE__);

    
        const std::vector<uint8_t> enc_key = key;

        int res;
        res = EVP_EncryptInit(ctx.get(), EVP_aes_256_cbc(), enc_key.data(), m_iv.data());
        throw_if_error(res, __FILE__, __LINE__);
        res = EVP_EncryptUpdate(ctx.get(), output.data(), &outlen, message.data(), inlen);
        throw_if_error(res, __FILE__, __LINE__);
        total_out += outlen;
        res = EVP_EncryptFinal(ctx.get(), output.data() + total_out, &outlen);
        throw_if_error(res, __FILE__, __LINE__);
        total_out += outlen;

        output.resize(total_out);
    }

    void decrypt(const std::vector<uint8_t>& key, const std::vector<uint8_t>& message, std::vector<uint8_t>& output) const {
        output.resize(message.size() * 3);
        int outlen = 0;
        size_t total_out = 0;

        EVP_CIPHER_CTX_t ctx(EVP_CIPHER_CTX_new());
        throw_if_error();

        
        const std::vector<uint8_t> enc_key = key;
        std::vector<uint8_t> target_message;
        std::vector<uint8_t> iv;

        iv = m_iv;
        target_message = message;

        int inlen = target_message.size();

        int res;
        res = EVP_DecryptInit(ctx.get(), EVP_aes_256_cbc(), enc_key.data(), iv.data());
        throw_if_error(res, __FILE__, __LINE__);
        res = EVP_DecryptUpdate(ctx.get(), output.data(), &outlen, target_message.data(), inlen);
        throw_if_error(res, __FILE__, __LINE__);
        total_out += outlen;
        res = EVP_DecryptFinal(ctx.get(), output.data() + outlen, &outlen);
        throw_if_error(res, __FILE__, __LINE__);
        total_out += outlen;

        output.resize(total_out);
    }
};

static std::vector<uint8_t> str_to_bytes(const std::string& message) {
    std::vector<uint8_t> out(message.size());
    for (size_t n = 0; n < message.size(); n++) {
        out[n] = message[n];
    }
    return out;
}

static std::string bytes_to_str(const std::vector<uint8_t>& bytes) {
    return std::string(bytes.begin(), bytes.end());
}

std::string encrypt_text(const std::string t) {

    const std::string iv = "1234567890123456";
    const std::string message = t;
    const std::string key = enckey;
    const aes256_cbc encryptor(str_to_bytes(iv));
    std::vector<uint8_t> enc_result; 
    encryptor.encrypt(str_to_bytes(key), str_to_bytes(message), enc_result);

    return bytes_to_str(enc_result);
}

std::string decrypt_text(const std::string t)
{
       
      const std::string iv = "1234567890123456";
      const std::string message = t;  
      const std::string key = enckey;
      const std::vector<uint8_t> msg = str_to_bytes(message);
      const aes256_cbc encryptor(str_to_bytes(iv));
      std::vector<uint8_t> enc_result;
      encryptor.decrypt(str_to_bytes(key), msg, enc_result);

      return bytes_to_str(enc_result);
 }

  