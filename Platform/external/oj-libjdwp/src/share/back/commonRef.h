/*
 * Copyright (c) 1998, 2005, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#ifndef JDWP_COMMONREF_H
#define JDWP_COMMONREF_H

void commonRef_initialize(void);
void commonRef_reset(JNIEnv *env);

jlong commonRef_refToID(JNIEnv *env, jobject ref);
jobject commonRef_idToRef(JNIEnv *env, jlong id);
void commonRef_idToRef_delete(JNIEnv *env, jobject ref);
jvmtiError commonRef_pin(jlong id);
jvmtiError commonRef_unpin(jlong id);
void commonRef_releaseMultiple(JNIEnv *env, jlong id, jint refCount);
void commonRef_release(JNIEnv *env, jlong id);
void commonRef_compact(void);

/* ANDROID-CHANGED: Called when an object is freed. This is called without any synchronization. */
void commonRef_handleFreedObject(jlong tag);

void commonRef_lock(void);
void commonRef_unlock(void);

#endif
