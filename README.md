# Steam-wallet-ransomware CLEAN
 It uses Steam wallet codes as ransom. Screen locker ransomware writed in C++.
 
## Disclaimer
This Ransomware should not be used to harm/threat/hurt others.
Its purpose is only to share knowledge and awareness about Malware/Operating Systems/Programming.
Steam wallet ransomware was created for learning about how programming can be used maliciously.

## How it works
When the 'Start.exe' file is run, it starts 'tor.exe' which makes a proxy to the Tor network. After that it checks if there is an internet connection and if the Tor proxy is turned on.
For 30 seconds, it checks if there are valid connections, if there are not, it fails.
If both connections are valid, it collects data such as: cpu, ram, system data, LAN ip and public ip. After that it generates a unique ID, which is sent to 's4m5vyfx72sjlsuzt72fabn3iv2igwovo3azn27im3xz3emt47gk6zad.onion' together with the collected data.
After they are sent, the 'ONloader.exe' program starts, which is the screen lock that makes it impossible to escape without payment.
It constantly checks the connections with the C2 server.To send the payment information must be entered in the data field. After they are sent, they are checked by a human and if they are correct the screen lock closes but if they are wrong he forcibly closes the computer through a BSOD.
The clean variant does not overwrite the MBR and does not create a bootloader.
##### The harmful variant is not public!

## Features
- Screen lock
- Disable Task Manager
- Kill Task Manager, Cmd and Explorer if they are opened.
- It encrypts the data sent to C2
- Communicate with C2 through a Tor proxy
- Unlocking is done remotely
- Countdown until the destruction date, but for this it is the CLEAN version, after it ends, nothing happens.
- Send data to server using http GET method
- Web Scraping to get values ​​for infection ID.

## Setup

- Make sure you have installed Visual Studio 2022 with C++ and an Apache server with phpmyadmin
- Configure a hidden onion service
- Create a c++ project for the Init program and add script files to it
- Create a c++ project for the ONloader program and add script files to it
- Create a c++ project for the String decryptor program and add script files to it
- Link dependencies
- Modifi C2 server url with your
- Build all.
- Put Init and ONloader in the same folder as tor.exe, start.ps1, images and dlls.(You can find 'tor.exe' in the Tor browser files)
- Put php files on apache server and modify connection dates
- Create a database and import tables
- Test it on VM
-To see the data sent to the server, it must be decrypted with the string descriptor
##### WARNING!!!Empty spaces in strings must be filled with '+' thus decryption will not work.
