/*
   (c) Copyright 2010 Sigma Designs

   Proprietary and confidential.
   */



#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdbool.h>

#include <directfb.h>
#include <directfb_keynames.h>
#include <directfb_util.h>

#include <direct/clock.h>
#include <direct/messages.h>

#include <sawman.h>

#ifndef __FULSCREENLAYOUT_H__
#define __FULSCREENLAYOUT_H__

struct WindowInfo
{
    DirectLink         link;
    SaWManWindowHandle window;
    bool               visible;
};

class LayoutBase
{
   public:
   LayoutBase(ISaWManManager *manager) : m_windows(NULL), m_manager(manager)
   { }

   virtual void Relayout() = 0;

   virtual void AddWindow(SaWManWindowHandle window);

   virtual void RemoveWindow(SaWManWindowHandle window);

   //virtual SaWManWindowHandle ActiveWindow() const;

   protected:
   DirectLink*   m_windows;
   ISaWManManager*  m_manager;
};

class FullScreenLayout : public LayoutBase
{
   public:
   FullScreenLayout(ISaWManManager* manager) : LayoutBase(manager)
   { }

   virtual void Relayout();
};

#endif

