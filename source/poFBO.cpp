/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  FBO.cpp
//  pocode
//
//  Created by Joshua Fisher on 10/17/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//

#include "poFBO.h"
#include "poTexture.h"
#include "poOpenGLState.h"

#include <boost/foreach.hpp>

namespace po {
    
    // -----------------------------------------------------------------------------------
    // ========================== Class: FBOConfig ==================================
    #pragma mark - FBOConfig -

    FBOConfig::FBOConfig()
    :	numMultisamples(0)
    ,	numColorBuffers(1)
    ,	textureConfig(GL_RGBA)
    {}
    
    
    //------------------------------------------------------------------------
    FBOConfig &FBOConfig::setNumMultisamples(uint nm) {
        numMultisamples = nm;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    FBOConfig &FBOConfig::setNumColorBuffers(uint ncb) {
        numColorBuffers = ncb;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    FBOConfig &FBOConfig::setColorBufferConfig(TextureConfig c) {
        textureConfig = c;
        return *this;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ========================== Class: FBO =============================================
    #pragma mark - FBO -

    FBO::FBO(uint w, uint h)
    :	width(w)
    ,	height(h)
    ,	cam(new Camera2D())
    ,	multisampling(false)
    {
        cam->setFixedSize(true, Point(w,h));
        setup();
    }

    FBO::FBO(uint w, uint h, const FBOConfig &c)
    :	width(w)
    ,	height(h)
    ,	config(c)
    ,	cam(new Camera2D())
    ,	multisampling(false)
    {
        cam->setFixedSize(true, Point(w,h));
        setup();
    }

    FBO::~FBO() {
        if(isValid())
            cleanup();
    }
    
    
    //------------------------------------------------------------------------
    ObjectModifier *FBO::copy() {
        FBO *fbo = new FBO(width, height, config);
        clone(fbo);
        return fbo;
    }
    
    
    //------------------------------------------------------------------------
    void FBO::clone(FBO* obj) {
        obj->setCamera(cam);
        ObjectModifier::clone(obj);
    }
    
    
    //------------------------------------------------------------------------
    bool FBO::isValid() const {
        return !framebuffers.empty() && framebuffers[0] > 0 && !colorbuffers.empty();
    }
    
    
    //------------------------------------------------------------------------
    Camera* FBO::getCamera() const {
        return cam;
    }
    
    
    //------------------------------------------------------------------------
    void FBO::setCamera(Camera *camera) {
        delete cam;
        cam = (Camera*)camera->copy();
    }
    
    
    //------------------------------------------------------------------------
    void FBO::reset(uint w, uint h, const FBOConfig &c) {
        if(isValid())
            cleanup();
        
        width = w;
        height = h;
        config = c;
        setup();
    }
    
    
    //------------------------------------------------------------------------
    // retrieve this texture to draw the FBO
    Texture *FBO::getColorTexture(uint idx) const {
        return colorbuffers[idx];
    }
    
    
    //------------------------------------------------------------------------
    Texture *FBO::getColorTextureCopy(uint idx) {
        Texture *col = colorbuffers[idx];
        Texture *tex = new Texture(col->getWidth(), col->getHeight(), NULL, col->getConfig());
        
        po::saveTextureState();
        glBindTexture(GL_TEXTURE_2D, tex->getUid());
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, col->getWidth(), col->getHeight());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        po::restoreTextureState();
        
        return tex;
    }
    
    
    //------------------------------------------------------------------------
    Texture *FBO::getDepthTexture() const {
        return NULL;
    }
    
    
    //------------------------------------------------------------------------
    void FBO::doSetUp(Object* obj) {
        po::saveTextureState();
        po::saveViewport();
        po::setViewport(0,0,width,height);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
        cam->setUp(obj);
    }
    
    
    //------------------------------------------------------------------------
    void FBO::doSetDown(Object* obj) {
        cam->setDown(obj);
        
        if(multisampling) {
        #ifdef OPENGL_ES
            #ifdef POTION_IOS
                glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, framebuffers[0]);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, framebuffers[1]);
                glResolveMultisampleFramebufferAPPLE();
            #else
                #warning non-ios opengl es fbo multisample implementation incomplete
            #endif
        #else
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffers[0]);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffers[1]);
            glBlitFramebuffer(0,0,width,height, 0,0,width,height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        #endif
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        po::restoreTextureState();
        po::restoreViewport();
    }
    
    
    //------------------------------------------------------------------------
    void FBO::setup() {
        int maxSamples =  po::maxFBOSamples();

        if(config.numMultisamples && maxSamples) {
            if(config.numMultisamples > maxSamples)
                config.numMultisamples = maxSamples;
            
            multisampling = true;

            renderbuffers.resize(1);
            glGenRenderbuffers(1, &renderbuffers[0]);
            
            // this is the multisample render buffer
            glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[0]);
            #ifdef POTION_IOS
                glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, config.numMultisamples, GL_RGBA8_OES, width, height);
            #else
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, config.numMultisamples, GL_RGBA8, width, height);
            #endif
            // we need 2 different framebuffers
            framebuffers.resize(2);
            glGenFramebuffers(2, &framebuffers[0]);
            // the first is the multisample buffer
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffers[0]);

            // attach it to the second framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[1]);

            // make new textures
            colorbuffers.clear();
            for(int i=0; i<config.numColorBuffers; i++) {
                colorbuffers.push_back(new Texture(width,height,NULL,config.textureConfig));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, colorbuffers[i]->getUid(), 0);
            }
            
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                printf("Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else {
            if(config.numMultisamples)
                printf("unable to do framebuffer multisampling\n");
            
            // we only need the one framebuffer
            framebuffers.resize(1);
            glGenFramebuffers(1, &framebuffers[0]);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[0]);
            
            colorbuffers.clear();
            
            for(int i=0; i<config.numColorBuffers; i++) {
                colorbuffers.push_back(new Texture(width,height,NULL,config.textureConfig));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, colorbuffers[i]->getUid(), 0);
            }

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                printf("Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
    
    
    //------------------------------------------------------------------------
    void FBO::cleanup() {
        BOOST_FOREACH(Texture *tex, colorbuffers)
            delete tex;
        colorbuffers.clear();

        // make sure none of this is bound right now
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(framebuffers.size(), &framebuffers[0]);
        glDeleteRenderbuffers(renderbuffers.size(), &renderbuffers[0]);
        framebuffers[0] = 0;
    }
}