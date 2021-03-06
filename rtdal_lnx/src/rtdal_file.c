/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

#include "defs.h"
#include "str.h"
#include "rtdal.h"
#include "rtdal_error.h"

static lstrdef(tmp_path);
static strdef(syspath);

/**
 * Sets the real filesystem relative path where rtdal_file_open() operates.
 * @param path Real file-system path.
 */
int rtdal_file_set_path(string path) {
	hdebug("path=%s\n",path);
	RTDAL_ASSERT_PARAM(path);

	struct stat st;
	if(stat(path,&st)) {
		RTDAL_SYSERROR("stat");
		return -1;
	}

	strcpy(syspath, path);

	return 0;
}

/**
 * Given a pathname for a file, rtdal_file_open() returns a file descriptor,
 * a nonnegative integer for use in rtdal_file_write(), rtdal_file_read() and
 * rtdal_file_close().
 * The function access the file-system directory configured by a previous call to
 * rtdal_file_set_path().
 *
 * @param name Name of the file
 * @return nonnegative integer on success, -1 on error.
 */
int rtdal_file_open(string name) {
	hdebug("name=%s\n",name);
	RTDAL_ASSERT_PARAM(name);
	int fd;


	sprintf(tmp_path, "%s/%s", syspath, name);

	fd = open(tmp_path, O_RDWR, O_CREAT | O_NONBLOCK);
	if (fd == -1) {
		RTDAL_SYSERROR("open");
		return -1;
	}
	return fd;
}

/**
 * Closes a previously opened file using rtdal_file_open()
 * @param fd File descriptor returned by rtdal_file_open()
 * @return zero on success, -1 on error.
 */
int rtdal_file_close(int fd) {
	hdebug("fd=%d\n",fd);
	RTDAL_ASSERT_PARAM(fd>0);
	if (close(fd)) {
		RTDAL_SYSERROR("open");
		return -1;
	}
	return 0;
}

/**
 * Writes up to size bytes from the buffer pointed by buf to the
 * file referred to by the file descriptor fd.
 *
 * @param fd File descriptor returned by rtdal_file_open()
 * @param buf Pointer to the memory to write to the file
 * @param size Number of bytes to write
 * @return On success, the number of bytes written is returned (zero indicates
 * nothing was written). On error, -1 is returned
 */
int rtdal_file_write(int fd, void* buf, int size) {
	hdebug("fd=%d, buf=0x%x, size=%d\n",fd, buf, size);
	RTDAL_ASSERT_PARAM(fd>0);
	RTDAL_ASSERT_PARAM(buf);
	RTDAL_ASSERT_PARAM(size>0);

	int s;

	s = write(fd, buf, (size_t) size);
	if (s == -1) {
		RTDAL_SYSERROR("write");
		return -1;
	}

	return s;
}

/**
 * Attemps to read up to size bytes from the the file descriptor fd into
 * the buffer starting at buf.
 *
 * @param fd File descriptor returned by rtdal_file_open()
 * @param buf Pointer to the memory to write to the file
 * @param size Number of bytes to write
 * @return On success, the number of bytes written is returned (zero indicates
 * nothing was written). On error, -1 is returned
 */
int rtdal_file_read(int fd, void* buf, int size) {
	RTDAL_ASSERT_PARAM(fd>0);
	RTDAL_ASSERT_PARAM(buf);
	RTDAL_ASSERT_PARAM(size>0);

	int s;

	s = read(fd, buf, (size_t) size);
	if (s == -1) {
		RTDAL_SYSERROR("read");
		return -1;
	}

	return s;
}
