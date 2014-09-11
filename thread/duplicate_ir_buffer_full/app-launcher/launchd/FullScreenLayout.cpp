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

#include "FullScreenLayout.h"

#define CHECK(x)                                  \
   do {                                         \
      DFBResult ret = (DFBResult)(x);         \
      if (ret != DFB_OK) {                    \
         DirectFBError(#x,ret);             \
         goto out;                          \
      }                                       \
   } while (0)


/***********************************************************************************/

void FullScreenLayout::Relayout()
{
   WindowInfo     *winfo;

   if( !m_windows )
      return;

   //manager = m_app_man->SaWManManager();
   D_ASSERT(m_manager != NULL);

   m_manager->Lock(m_manager);

   direct_list_foreach(winfo, m_windows)
   {
      SaWManWindowHandle        window = winfo->window;
      SaWManProcess             process;
      DFBBoolean                visible = DFB_FALSE;

      if( m_manager->GetProcessInfo(m_manager, window, &process) )
      {
         D_ERROR("SaWMan/AppLauncher: Couldn't get process info for window %#x !\n", (int)window);
         continue;
      }

      /* Only the active process' windows are visible (if not manually hidden); all others must be hidden */
//      if (process.pid == m_app_man->ActivePID() && winfo->visible)
         visible = DFB_TRUE;

      if( m_manager->SetVisible(m_manager, window, visible) )
      {
         D_ERROR("SaWMan/AppLauncher: Couldn't show/hide window %#x !\n", (int)window);
         continue;
      }
   }

   m_manager->QueueUpdate(m_manager, DWSC_MIDDLE, NULL);

   m_manager->Unlock(m_manager);
}

void LayoutBase::AddWindow(SaWManWindowHandle window)
{
   WindowInfo     *winfo;

   D_ASSERT(m_manager != NULL);

   winfo = (WindowInfo *)D_CALLOC(1, sizeof(WindowInfo));
   if (!winfo)
   {
      D_OOM();
      return;
   }

   m_manager->Lock(m_manager);

   if (m_manager->InsertWindow(m_manager, window, SAWMAN_WINDOW_NONE, SWMWR_TOP))
   {
      D_ERROR("SaWMan/AppLauncher: Couldn't add window %#x !\n", (int)window);
      D_FREE(winfo);
   }
   else
   {
      winfo->window  = window;
      winfo->visible = DFB_TRUE;
      direct_list_append(&m_windows, &winfo->link);

      Relayout();
   }

   m_manager->Unlock(m_manager);
}

void LayoutBase::RemoveWindow(SaWManWindowHandle  window)
{
   WindowInfo     *winfo;

   D_ASSERT(m_manager != NULL);

   m_manager->Lock(m_manager);

   direct_list_foreach(winfo, m_windows)
      if (winfo->window == window)
         break;

   D_ASSERT(winfo);

   if (winfo)
   {
      /* Remove window from layout. */
      m_manager->RemoveWindow(m_manager, window);

      direct_list_remove(&m_windows, &winfo->link);
      D_FREE(winfo);

      Relayout();
   }

   m_manager->Unlock(m_manager);
}

#if 0
SaWManWindowHandle LayoutBase::ActiveWindow() const
{
   ISaWManManager     *manager;
   WindowInfo         *wnd;
   SaWManWindowHandle  ret_wnd = SAWMAN_WINDOW_NONE;

   D_ASSERT(m_app_man);

   if (!m_app_man->m_windows)
      return ret_wnd;

   manager = m_app_man->SaWManManager();
   D_ASSERT(manager != NULL);

   manager->Lock(manager);

   direct_list_foreach(wnd, m_app_man->m_windows)
   {
      SaWManWindowHandle window = wnd->window;
      SaWManWindowInfo   info;

      if (manager->GetWindowInfo(manager, window, &info))
      {
         D_ERROR("SaWMan/AppLauncher: Couldn't get window info for window %#x !\n", (int)window);
         continue;
      }

      if (info.flags & SWMWF_FOCUSED)     // active window for its process
      {
         SaWManProcess process;

         if (manager->GetProcessInfo(manager, window, &process))
         {
            D_ERROR("SaWMan/AppLauncher: Couldn't get process info for window %#x !\n", (int)window);
            continue;
         }

         if (process.pid == m_app_man->ActivePID())
         {
            ret_wnd = window;
            break;
         }

      }
   }

   manager->Unlock(manager);
   return ret_wnd;
}
#endif

