/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com 
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef ResourceHandleManager_h
#define ResourceHandleManager_h

#include "Frame.h"
#include "Timer.h"
#include "ResourceHandleClient.h"
#include <curl/curl.h>

namespace WebCore {

class ResourceHandleList {
public:
    ResourceHandleList(ResourceHandle* job, ResourceHandleList* next)
        : m_job(job)
        , m_next(next)
        , m_removed(false)
    {}
    ResourceHandleList* next() const { return m_next; }
    ResourceHandle* job() const { return m_job; }
    void setRemoved(bool removed) { m_removed = removed; }
    bool removed() const { return m_removed; }

private:
    ResourceHandle* m_job;
    ResourceHandleList* m_next;
    bool m_removed;
};

class ResourceHandleManager {
public:
    static ResourceHandleManager* sharedInstance();
    void add(ResourceHandle*);
    void cancel(ResourceHandle*);
    void setCookieJarFileName(const char* cookieJarFileName);

    void setupPOST(ResourceHandle*);
    void setupPUT(ResourceHandle*);

private:
    ResourceHandleManager();
    void downloadTimerCallback(Timer<ResourceHandleManager>*);
    void removeFromCurl(ResourceHandle*);
    bool removeScheduledJob(ResourceHandle*);
    void startJob(ResourceHandle*);
    bool startScheduledJobs();

    Timer<ResourceHandleManager> m_downloadTimer;
    CURLM* m_curlMultiHandle; // FIXME: never freed
    CURLSH* m_curlShareHandle; // FIXME: never freed
    char* m_cookieJarFileName; // FIXME: never freed
    char m_curlErrorBuffer[CURL_ERROR_SIZE];
    ResourceHandleList* m_resourceHandleListHead;
};

}

#endif
