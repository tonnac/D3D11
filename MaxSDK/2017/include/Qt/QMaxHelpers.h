//-----------------------------------------------------------------------------
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.
//-----------------------------------------------------------------------------

#pragma once

#include "../CoreExport.h"
#include "QtMax.h"

#include <QtGui/QWidget>
#include <QtCore/QObject>

class Bitmap;


namespace QtHelpers
{

   ////////////////////////////////////////////////////////////////////////////
   // Image functions.

	//Takes the name of the icon to load directory/iconName.png. It loads all versions of the 
   //icon, example: directory/iconName_XX.png, directory/iconName_on_XX.png where XX is the size
   //of the icon.
   //Input Parameters: iconName - The name of the icon to load.
   //OutputParameters: numIcons- the number of elements in the returned array.
   //Return Value    : An array of HIcon containing the HIcons of the loaded icons. Must be destroyed 
   //                  by the caller function.
   //Note: The returned HICON array must be destroyed by the user. Also, the user has to call DestroyIcon
   //      to destroy the icon when not used anymore.
   MaxQTExport HICON* CreateHIconsFromMultiResIcon(const MCHAR* iconName, int& numIcons);

   // Convert a 3ds Max SDK bitmap to a Qt icon.
   MaxQTExport QIcon QIconFromBitmap( Bitmap* bm );

   // Convert a Qt icon to a 3ds Max SDK bitmap.
   MaxQTExport Bitmap* BitmapFromQIcon( const QIcon& icon, int width, int height, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off );

   // Convert a Qt icon to a HBITMAP.
   MaxQTExport HBITMAP HBITMAPFromQIcon( const QIcon& icon, int width, int height, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off );

   /* \brief Associates a help context identifier with a given widget.
   * Equivalent to the windows method SetWindowContextHelpId().
   * Use this function for registering a help context id on a qwidget without 
   * making it native win32 window.
   * \param widget The widget with which the help context id should be associated.
   * \param contextId The Help context identifier.
   */
   MaxQTExport void SetWindowContextHelpId( QWidget* widget, int contextId );

   /* \brief Captures the mouse input.
    * The widget will receive all mouse input events until widget->releaseMouse() is called.
	* \param widget The widget that will receive all mouse events.
	* \note Please use this helper function instead of using directly widget->grabMouse().
	* The standard QWidget mouse grab function of Qt4.8. can cause the freezing of 
	* the workstation due the usage of an internal windows hook.
	*/
   UtilExport void grabMouse( QWidget* widget );

   /* \brief Captures the mouse input and changes the current cursor.
    * The widget will receive all mouse input events until widget->releaseMouse() is called.
	* \param widget The widget that will receive all mouse events.
	* \param cursor The cursor that will be displayed during the mouse capture.
	* \note Please use this helper function instead of using directly widget->grabMouse().
	* The standard QWidget mouse grab function of Qt4.8. can cause the freezing of 
	* the workstation due the usage of an internal windows hook.
	*/
   UtilExport void grabMouse( QWidget* widget, const QCursor &cursor );


}; // end of QtHelpers namespace -------------------------------------------------
