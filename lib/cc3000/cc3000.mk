# List of all the board related files.
CC3000SRC = ${CC3000}/Socket/Endpoint.cpp \
            ${CC3000}/Socket/Socket.cpp \
            ${CC3000}/Socket/TCPSocketConnection.cpp \
            ${CC3000}/Socket/TCPSocketServer.cpp \
            ${CC3000}/Socket/UDPSocket.cpp \
            ${CC3000}/cc3000_event.cpp \
            ${CC3000}/cc3000_hci.cpp \
            ${CC3000}/cc3000_netapp.cpp \
            ${CC3000}/cc3000_nvmem.cpp \
            ${CC3000}/cc3000_security.cpp \
            ${CC3000}/cc3000_simplelink.cpp \
            ${CC3000}/cc3000_socket.cpp \
            ${CC3000}/cc3000_spi.cpp \
            ${CC3000}/cc3000_wlan.cpp \
            ${CC3000}/cc3000.cpp

# Required include directories
CC3000INC = ${CC3000} \
            ${CC3000}/Helper \
            ${CC3000}/Socket
