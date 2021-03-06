/*
 * socks5.h - Define SOCKS5's header
 *
 * Copyright (C) 2013, clowwindy <clowwindy42@gmail.com>
 *
 * This file is part of the shadowsocks-libev.
 *
 * shadowsocks-libev is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * shadowsocks-libev is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with shadowsocks-libev; see the file COPYING. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * https://fossies.org/dox/socat-2.0.0-b9/xio-socks5_8h_source.html
 *
 * https://en.wikipedia.org/wiki/SOCKS
 * https://www.ietf.org/rfc/rfc1928.txt - NO_AUTH SOCKS5
 * https://www.ietf.org/rfc/rfc1929.txt - USERNAME/PASSWORD SOCKS5
 *
 * https://github.com/brozeph/simple-socks/blob/master/lib/socks5.js
 * https://github.com/mfontanini/Programs-Scripts/blob/master/socks5/socks5.cpp
 * https://github.com/isayme/socks5
 *
 */

#ifndef _SOCKS5_H
#define _SOCKS5_H

#include <stdint.h> // for uint16_t
#include <stddef.h> // for size_t

#define SOCKS5_VERSION          0x05

#define SOCKS5_METHOD_NOAUTH    0x00
#define SOCKS5_METHOD_GSSAPI    0x01
#define SOCKS5_METHOD_USERPASS  0x02
#define SOCKS5_METHOD_AVENTAIL  0x86
#define SOCKS5_METHOD_NONE      0xff

#define SOCKS5_COMMAND_CONNECT  0x01
#define SOCKS5_COMMAND_BIND     0x02
#define SOCKS5_COMMAND_UDPASSOC 0x03

#define SOCKS5_ADDRTYPE_IPV4    0x01
#define SOCKS5_ADDRTYPE_NAME    0x03
#define SOCKS5_ADDRTYPE_IPV6    0x04

#define SOCKS5_REPLY_SUCCESS    0x00
#define SOCKS5_REPLY_FAILURE    0x01
#define SOCKS5_REPLY_DENIED     0x02
#define SOCKS5_REPLY_NETUNREACH 0x03
#define SOCKS5_REPLY_HOSTUNREACH 0x04
#define SOCKS5_REPLY_REFUSED    0x05
#define SOCKS5_REPLY_TTLEXPIRED 0x06
#define SOCKS5_REPLY_CMDUNSUPP  0x07
#define SOCKS5_REPLY_ADDRUNSUPP 0x08

#define SOCKS5_USERPASS_VERSION 0x01

#pragma pack(push)
#pragma pack(1)


/**
 * +----+----------+----------+
 * |VER | NMETHODS | METHODS  |
 * +----+----------+----------+
 * | 1  |    1     | 1 to 255 |
 * +----+----------+----------+
 **/
struct method_select_request {
    char ver;
    char nmethods;
    char methods[0];
};


struct method_select_response {
    char ver;
    char method;
};


/**
 * +----+------+----------+------+----------+
 * |VER | ULEN |  UNAME   | PLEN |  PASSWD  |
 * +----+------+----------+------+----------+
 * | 1  |  1   | 1 to 255 |  1   | 1 to 255 |
 * +----+------+----------+------+----------+
 **/
struct socks5_authenticate {
    char ver;
    char ulen;
    char uname_n_others[0];
};


/**
 * +----+-----+-------+------+----------+----------+
 * |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
 * +----+-----+-------+------+----------+----------+
 * | 1  |  1  | X'00' |  1   | Variable |    2     |
 * +----+-----+-------+------+----------+----------+
 **/
struct socks5_request {
    char ver;
    char cmd;
    char rsv;
    char addr_type;
    char addr_n_port[0];
};


/**
 * +----+-----+-------+------+----------+----------+
 * |VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
 * +----+-----+-------+------+----------+----------+
 * | 1  |  1  | X'00' |  1   | Variable |    2     |
 * +----+-----+-------+------+----------+----------+
 **/
struct socks5_response {
    char ver;
    char rep;
    char rsv;
    char addr_type;
    char addr_n_port[0];
};

#pragma pack(pop)

struct socks5_request *
build_socks5_request(const char *host, uint16_t port, char *buffer, size_t buffer_size, size_t *data_size);

struct method_select_response *
build_socks5_method_select_response(int method, char *buffer, size_t buffer_size);

struct socks5_response *
build_socks5_response(int rep, int addr_type, struct sockaddr_in *addr, char *buffer, size_t buffer_size, size_t *data_size);

#endif // _SOCKS5_H
