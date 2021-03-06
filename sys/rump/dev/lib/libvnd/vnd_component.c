/*	$NetBSD: vnd_component.c,v 1.3 2016/07/26 01:49:49 pgoyette Exp $	*/

/*
 * Copyright (c) 2009 Antti Kantee.  All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: vnd_component.c,v 1.3 2016/07/26 01:49:49 pgoyette Exp $");

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/device.h>
#include <sys/stat.h>

#include <rump-sys/kern.h>
#include <rump-sys/vfs.h>

RUMP_COMPONENT(RUMP_COMPONENT_DEV)
{
	extern const struct bdevsw vnd_bdevsw;
	extern const struct cdevsw vnd_cdevsw;
	extern devmajor_t vnd_bmajor, vnd_cmajor;
	int error;

	/* go, mydevfs */

	if ((error = devsw_attach("vnd", &vnd_bdevsw, &vnd_bmajor,
	    &vnd_cdevsw, &vnd_cmajor)) != 0)
		panic("cannot attach vnd: %d", error);

	if ((error = rump_vfs_makedevnodes(S_IFBLK, "/dev/vnd0", 'a',
	    vnd_bmajor, 0, 7)) != 0)
		panic("cannot create cooked vnd dev nodes: %d", error);
	if ((error = rump_vfs_makedevnodes(S_IFCHR, "/dev/rvnd0", 'a',
	    vnd_cmajor, 0, 7)) != 0)
		panic("cannot create raw vnd dev nodes: %d", error);

	devsw_detach(&vnd_bdevsw, &vnd_cdevsw);
}
