#pragma once
auto EncodeBase64(const std::string& to_encode) -> std::string;
auto DecodeBase64(const std::string& to_decode) -> std::string;
std::string encrypt_text(const std::string t);
std::string decrypt_text(const std::string t);