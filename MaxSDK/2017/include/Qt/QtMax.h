//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2015 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../strclass.h"
#include "../acolor.h"

#pragma warning(push)
#pragma warning(disable: 4127 4251 4275 4512 4800 )
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#pragma warning(pop)

#ifdef BLD_MAXQTBRIDGE
#define MaxQTExport __declspec( dllexport )
#else
#define MaxQTExport __declspec( dllimport )
#endif

/*! \file
 * \warning This API is unsupported and provided for experiment only.  It should not be used for production.
 * This API will change or disappear in a future release.
 */
namespace MaxSDK
{;

// Enable AColor to be used in a QVariant
Q_DECLARE_METATYPE(AColor);

// Retrieve localized text from its resource ID.
QString QStringFromID(int resourceID);

}     // namespace

#include "QtMax.inline.h"