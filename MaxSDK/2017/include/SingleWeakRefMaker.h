//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.
//**************************************************************************/
// DESCRIPTION: Utility base class for a ReferenceMaker with a single target 
// that does not share ownership of it's ReferenceTarget.
// AUTHOR: Nicolas Desjardins from an original implementation by Larry Minton
// DATE: 2006-08-22
//***************************************************************************/

#pragma once

#include "ref.h"


namespace MaxSDK
{

/*!
 * \brief Utility base class for a ReferenceMaker with a single target that
 * does not share ownership of it's ReferenceTarget.
 *
 * \see SingleRefMaker
 */
class SingleWeakRefMaker : public SingleRefMaker
{
public:
	CoreExport static const SClass_ID SUPER_CLASS_ID;

	/*!
	 * \brief Constructor.
	 */
	CoreExport SingleWeakRefMaker();

	/*!
	 * \brief Destructor.
	 */
	CoreExport virtual ~SingleWeakRefMaker();

	/*!
	 * \brief Get this class's super class ID.
	 *
	 * \return Returns SingleWeakRefMaker::CLASS_ID
	 */
	CoreExport virtual SClass_ID SuperClassID();
	
	/*!
	 * \brief Returns false to indicate that it does not share ownership.
	 *
	 * \param target The ReferenceTarget in question.
	 * \return Always returns false.
	 */
	CoreExport virtual BOOL IsRealDependency(ReferenceTarget* target);
	
	/*!
	 * \brief Notifies this reference maker that one of its ReferenceTargets has
	 * changed.
	 *
	 * The SingleWeakRefMaker supplies a default implementation to track when the
	 * reference gets deleted.
	 */
	CoreExport virtual RefResult NotifyRefChanged( 
		const Interval& changeInt, 
		RefTargetHandle hTarget, 
		PartID& partID, 
		RefMessage message, 
		BOOL propagate );

	/*!
	 * \brief DeleteThis method to be called instead of deleting directly.
	 */
	CoreExport virtual void DeleteThis();

	/*!
	 * \brief The class name.
	 */
	virtual void GetClassName(MSTR& s) { s = _M("MaxSDK.SingleWeakRefMaker"); } // from Animatable

};

}

