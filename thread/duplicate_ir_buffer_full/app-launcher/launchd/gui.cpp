/*
   (c) Copyright 2010 Sigma Designs

   Proprietary and confidential.
   */

//#include <config.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <termios.h>
#include <unistd.h>

#include <linux/input.h>
#include <linux/keyboard.h>

#include <stdio.h>
#include <string.h>

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

#include <tinyxml.h>

extern "C" {
#include <divine.h>
}
#include "gui.h"

#define UNUSED( x ) do{ x = x; }while(0)

#define CHECK(x)                                  \
   do {                                         \
      DFBResult ret = (DFBResult)(x);         \
      if (ret != DFB_OK) {                    \
         DirectFBError(#x,ret);             \
         goto out;                          \
      }                                       \
   } while (0)

#define DRAWING_POS_X 100
#define DRAWING_POS_Y 100


DFBResult GUI::Initialize()
{
   IDirectFBScreen *dfb_osd_screen = NULL;
   int drawing_pos_x = -1;
   int drawing_pos_y = -1;
   int menu_separator_width = -1;
   float width_raport = 1;

   CHECK(m_app_man->m_dfb->GetDisplayLayer(m_app_man->m_dfb, DLID_PRIMARY, &m_layer));
   //CHECK(m_layer->SetCooperativeLevel(m_layer, DLSCL_ADMINISTRATIVE));

   // Get screen size
   // We are taking the osd screen size
   CHECK(m_app_man->m_dfb->GetScreen(m_app_man->m_dfb, 0, &dfb_osd_screen));
   CHECK(dfb_osd_screen->GetSize(dfb_osd_screen, &m_app_man->m_screen_width, &m_app_man->m_screen_height));
   if (m_app_man->m_screen_width <= 0 || m_app_man->m_screen_height <= 0)
   {
      D_ASSERT(false);
      m_app_man->m_screen_width  = 1920;
      m_app_man->m_screen_height = 1080;
   }
   // set the drawing pos by the report of the screen 
   width_raport = m_app_man->m_screen_width / 1920.0;
   drawing_pos_x = DRAWING_POS_X * width_raport;
   drawing_pos_y = DRAWING_POS_Y * m_app_man->m_screen_height / 1080;
   menu_separator_width = 10 * width_raport;

   m_app_list_reg.x1 = drawing_pos_x;
   m_app_list_reg.y1 = drawing_pos_y;
   m_app_list_reg.x2 = m_app_man->m_screen_width / 4 - 1;
   m_app_list_reg.y2 = m_app_man->m_screen_height - drawing_pos_y - 1;

   m_app_details_reg.x1 = m_app_list_reg.x2 + menu_separator_width + 1;
   m_app_details_reg.y1 = m_app_list_reg.y1;
   m_app_details_reg.x2 = m_app_man->m_screen_width - drawing_pos_x - 1;
   m_app_details_reg.y2 = m_app_list_reg.y2;

   DFBWindowDescription wdsc;

   wdsc.flags  = (DFBWindowDescriptionFlags)( DWDESC_POSX | DWDESC_POSY |
         DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS );
   wdsc.posx   = 0;
   wdsc.posy   = 0;
   wdsc.width  = m_app_man->m_screen_width;
   wdsc.height = m_app_man->m_screen_height;
   wdsc.caps   = DWCAPS_NODECORATION;

   CHECK(m_layer->CreateWindow(m_layer, &wdsc, &m_wnd));
   CHECK(m_wnd->SetOpacity(m_wnd, 0xff));
   CHECK(m_wnd->SetStackingClass(m_wnd, DWSC_UPPER));

   CHECK(m_wnd->GetSurface(m_wnd, &m_surf));

   // set up font
   char font_filename[1024];
   if (FONTNAME[0] != '/')
   {
      char *dfbDir = getenv("DCCHD_INSTALL_DIR");
      if (dfbDir)
      {
         strcpy(font_filename, dfbDir);
         strcat(font_filename, "/share/directfb-examples/fonts/");
      }
   }
   strcat(font_filename, FONTNAME);

   DFBFontDescription fdsc;
   fdsc.flags = DFDESC_HEIGHT;
   fdsc.height = m_app_man->m_screen_height/30;
   CHECK(m_app_man->m_dfb->CreateFont(m_app_man->m_dfb, font_filename, &fdsc, &m_font_big));

   fdsc.height = m_app_man->m_screen_height/40;
   CHECK(m_app_man->m_dfb->CreateFont(m_app_man->m_dfb, font_filename, &fdsc, &m_font_medium));

   check_and_set_app_ptrs();

   return DFB_OK;

out:
   return DFB_FAILURE;
}

void GUI::Cleanup()
{
   if (m_font_big)
   {
      m_font_big->Release(m_font_big);
      m_font_big = NULL;
   }
   if (m_font_medium)
   {
      m_font_medium->Release(m_font_medium);
      m_font_medium = NULL;
   }
   if (m_surf)
   {
      m_surf->Release(m_surf);
      m_surf = NULL;
   }
   if (m_wnd)
   {
      m_wnd->Release(m_wnd);
      m_wnd = NULL;
   }
   if (m_layer)
   {
      m_layer->Release(m_layer);
      m_layer = NULL;
   }
}

void GUI::DrawCurrentApp(bool flip)
{
   if (!check_and_set_app_ptrs())
      return;     // no applications in list

   draw_app((Application *)m_current_app);
   if (flip)
      m_surf->Flip(m_surf, NULL, DSFLIP_NONE);
}

void GUI::draw_app(Application *app)
{
   int x = m_app_details_reg.x1;
   int y = m_app_details_reg.y1;
   int h_big, h_medium;
   char str[1024];

   m_surf->SetClip(m_surf, &m_app_details_reg);
   m_surf->Clear(m_surf, 0, 0, 0, 0xff);

   m_font_big->GetHeight(m_font_big, &h_big);
   m_font_medium->GetHeight(m_font_medium, &h_medium);

   m_surf->SetColor(m_surf, 0xff, 0xff, 0xff, 0xff);
   m_surf->SetFont(m_surf, m_font_big);
   m_surf->DrawString(m_surf, app->name, -1, x, y, DSTF_TOPLEFT);
   y += h_big + h_big;

   m_surf->SetFont(m_surf, m_font_medium);

   if (app->path)
   {
      snprintf(str, sizeof(str), "Path: %s", app->path);
      m_surf->DrawString(m_surf, str, -1, x, y, DSTF_TOPLEFT);
      y += h_medium;
   }

   snprintf(str, sizeof(str), "Command line: %s", app->args);
   m_surf->DrawString(m_surf, str, -1, x, y, DSTF_TOPLEFT);
   y += h_medium;

   snprintf(str, sizeof(str), "Type: %s",
         app->comm->Type() == AT_DirectFB ? "DirectFB" :
         app->comm->Type() == AT_RUA ? "RUA" :
         "<unknown>");
   m_surf->DrawString(m_surf, str, -1, x, y, DSTF_TOPLEFT);
   y += h_medium;

   snprintf(str, sizeof(str), "Running: %s", app->started ? "yes" : "no");
   m_surf->DrawString(m_surf, str, -1, x, y, DSTF_TOPLEFT);
   y += h_medium;

   snprintf(str, sizeof(str), "Last start time: %ld.%03lds", (long)(app->start_time / 1000), (long)(app->start_time % 1000));
   m_surf->DrawString(m_surf, str, -1, x, y, DSTF_TOPLEFT);
   y += h_medium;

   if (app->started)
   {
      snprintf(str, sizeof(str), "PID: %d", app->pid);
      m_surf->DrawString(m_surf, str, -1, x, y, DSTF_TOPLEFT);
      y += h_medium;

      if (app->process)
      {
         SaWManProcess *process = app->process;

         m_surf->DrawString(m_surf, "Process data:", -1, x, y, DSTF_TOPLEFT);
         y += h_medium;
         x += 32;

         snprintf(str, sizeof(str), "Fusion ID: %lu", process->fusion_id);
         m_surf->DrawString(m_surf, str, -1, x, y, DSTF_TOPLEFT);
         y += h_medium;

         snprintf(str, sizeof(str), "PID: %d", process->pid);
         m_surf->DrawString(m_surf, str, -1, x, y, DSTF_TOPLEFT);
         y += h_medium;

         //x -= 32;
      }
   }

   if (app->icon)
   {
      int i_w, i_h;

      app->icon->GetSize(app->icon, &i_w, &i_h);

      m_surf->SetBlittingFlags(m_surf, DSBLIT_NOFX);
      m_surf->Blit(m_surf, app->icon, NULL, m_app_details_reg.x2 - i_w - 9, m_app_details_reg.y2 - i_h - 10);
   }
}

bool GUI::check_and_set_app_ptrs()
{
   if (!m_first_app)
      m_first_app = m_app_man->m_applications;
   if (!m_current_app)
      m_current_app = m_first_app;

   return m_first_app && m_current_app;
}

void GUI::DrawAppList(bool flip)
{
   if (!check_and_set_app_ptrs())
      return;     // no applications in list

   int x = m_app_list_reg.x1;
   int y = m_app_list_reg.y1;
   int h_font;
   int w_minus_1;
   int border_size;
   Application *app;

   w_minus_1 = m_app_list_reg.x2 - m_app_list_reg.x1;

   m_surf->SetClip(m_surf, &m_app_list_reg);
   m_surf->Clear(m_surf, 0x30, 0x30, 0x30, 0xff);

   m_font_big->GetHeight(m_font_big, &h_font);
   m_surf->SetFont(m_surf, m_font_big);

   border_size = h_font / 2;

   direct_list_foreach(app, m_first_app)
   {
      DFBRegion clip = {x, y, x+w_minus_1, y+h_font+2*border_size - 1};

      m_surf->SetClip(m_surf, &clip);

      if (&app->link == m_current_app)
         m_surf->Clear(m_surf, 0, 0, 0, 0xff);

      m_surf->SetColor(m_surf, 0xff, 0xff, 0xff, 0xff);
      m_surf->DrawString(m_surf, app->name, -1, x+border_size, y+border_size, DSTF_TOPLEFT);
      y += h_font + 2 * border_size;
   }

   if (flip)
      m_surf->Flip(m_surf, NULL, DSFLIP_NONE);
}

void GUI::Redraw()
{
   m_surf->SetClip(m_surf, NULL);
   m_surf->Clear(m_surf, 128, 128, 128, 128);
   DrawAppList(false);
   DrawCurrentApp(false);
   m_surf->Flip(m_surf, NULL, DSFLIP_NONE);
}

void GUI::Show(bool show)
{
   D_ASSERT(m_wnd);
   m_wnd->SetOpacity(m_wnd, show ? 0xff : 0);
}

void GUI::SelectApp(int relative_pos)
{
   if (!check_and_set_app_ptrs())
      return;     // no applications in list

   D_ASSERT(m_first_app);
   D_ASSERT(m_current_app);

   int h_font;
   int n_apps;
   DirectLink *new_first_app = m_first_app;

   m_font_big->GetHeight(m_font_big, &h_font);
   n_apps = (m_app_list_reg.y2 - m_app_list_reg.y1 + h_font) / h_font;

   // calculate the new current app
   while (relative_pos && m_current_app)
   {
      if (relative_pos < 0)
      {
         m_current_app = m_current_app->prev;
         ++relative_pos;
      }
      else
      {
         m_current_app = m_current_app->next;
         --relative_pos;
      }
   }

   // make sure the current app is visible (move the first app if necessary)
   direct_list_foreach_reverse(new_first_app, m_current_app)
   {
      if (--n_apps == 0)
         break;
      if (new_first_app == m_first_app)
         break;
   }

   m_first_app = new_first_app;

   DrawAppList(false);
   DrawCurrentApp();
}

