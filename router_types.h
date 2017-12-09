/*
 * Copyright (C) Marek Lindner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 3 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 *
 * SPDX-License-Identifier: GPL-3.0+
 * License-Filename: LICENSES/preferred/GPL-3.0
 */

#ifndef __AP51_FLASH_ROUTER_TYPES_H__
#define __AP51_FLASH_ROUTER_TYPES_H__

#include <stdint.h>

struct node;

int router_types_init(void);
void router_types_detect_pre(const uint8_t *our_mac);
int router_types_detect_main(struct node *node, const char *packet_buff,
			     int packet_buff_len);

extern int router_types_priv_size;

#endif /* __AP51_FLASH_ROUTER_TYPES_H__ */
