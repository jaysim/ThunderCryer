/* Copyright (C) 2013 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "TCPSocketServer.h"
#include "string.h"

TCPSocketServer::TCPSocketServer() {

}

int TCPSocketServer::bind(int port) {
    if (init_socket(SOCK_STREAM, IPPROTO_TCP) < 0) {
        return -1;
    }

    sockaddr_in localHost;
    memset(&localHost, 0, sizeof(localHost));

    localHost.sin_family = AF_INET;
    localHost.sin_port = htons(port);
    localHost.sin_addr.s_addr = 0;

    if (_cc3000_module->_socket.bind(_sock_fd, (const sockaddr *)&localHost, sizeof(localHost)) < 0) {
        close();
        return -1;
    }

    return 0;
}

int TCPSocketServer::listen(int max) {
    if (_sock_fd < 0) {
        return -1;
    }

    if (_cc3000_module->_socket.listen(_sock_fd, max) < 0) {
        close();
        return -1;
    }

    return 0;
}


int TCPSocketServer::accept(TCPSocketConnection& connection) {
    if (_sock_fd < 0) {
        return -1;
    }

    if (!_blocking) {
        TimeInterval timeout(_timeout);
        if (wait_readable(timeout) != 0) {
            return -1;
        }
    }

    connection.reset_address();
    socklen_t newSockRemoteHostLen = sizeof(connection._remote_host);
    int fd = _cc3000_module->_socket.accept(_sock_fd, (sockaddr *) &connection._remote_host, &newSockRemoteHostLen);
    if (fd < 0) {
        return -1;
    }
    /* s_addr is returned in the little endian */
    connection._remote_host.sin_addr.s_addr = htonl(connection._remote_host.sin_addr.s_addr);
    connection._sock_fd = fd;
    connection._is_connected = true;

    return 0;
}
