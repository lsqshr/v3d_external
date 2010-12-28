/*
 * Copyright (c)2006-2010  Hanchuan Peng (Janelia Farm, Howard Hughes Medical Institute).
 * All rights reserved.
 */


/************
 ********* LICENSE NOTICE ************
 
 This folder contains all source codes for the V3D project, which is subject to the following conditions if you want to use it.
 
 You will ***have to agree*** the following terms, *before* downloading/using/running/editing/changing any portion of codes in this package.
 
 1. This package is free for non-profit research, but needs a special license for any commercial purpose. Please contact Hanchuan Peng for details.
 
 2. You agree to appropriately cite this work in your related studies and publications.
 
 Peng, H., Ruan, Z., Long, F., Simpson, J.H., and Myers, E.W. (2010) “V3D enables real-time 3D visualization and quantitative analysis of large-scale biological image data sets,” Nature Biotechnology, Vol. 28, No. 4, pp. 348-353, DOI: 10.1038/nbt.1612. ( http://penglab.janelia.org/papersall/docpdf/2010_NBT_V3D.pdf )
 
 Peng, H, Ruan, Z., Atasoy, D., and Sternson, S. (2010) “Automatic reconstruction of 3D neuron structures using a graph-augmented deformable model,” Bioinformatics, Vol. 26, pp. i38-i46, 2010. ( http://penglab.janelia.org/papersall/docpdf/2010_Bioinfo_GD_ISMB2010.pdf )
 
 3. This software is provided by the copyright holders (Hanchuan Peng), Howard Hughes Medical Institute, Janelia Farm Research Campus, and contributors "as is" and any express or implied warranties, including, but not limited to, any implied warranties of merchantability, non-infringement, or fitness for a particular purpose are disclaimed. In no event shall the copyright owner, Howard Hughes Medical Institute, Janelia Farm Research Campus, or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; reasonable royalties; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.
 
 4. Neither the name of the Howard Hughes Medical Institute, Janelia Farm Research Campus, nor Hanchuan Peng, may be used to endorse or promote products derived from this software without specific prior written permission.
 
 *************/


/*
 * basic_triview.h
 *
 *  Created on: Dec 08, 2010
 *      Author: Yang Yu, Hanchuan Peng
 */

#ifndef __BASIC_TRIVIEW_H__
#define __BASIC_TRIVIEW_H__

#include "v3d_basicdatatype.h"

class TriviewControl
{
public:
	virtual ~TriviewControl() {};

public:
	
// 2.2 functions. some function declaration defined by Hanchuan Peng for tri-view control. 2010-Dec-08
	virtual void getFocusLocation(V3DLONG & cx, V3DLONG & cy, V3DLONG & cz) const = 0;
	virtual void setFocusLocation(V3DLONG cx, V3DLONG cy, V3DLONG cz)  = 0;
	
	virtual void setFocusLocation2Center() = 0;
	
	virtual void updateMinMax(V3DLONG nFrame) = 0;
 	
	virtual void getTriViewColorDispType(int & mytype) = 0;
	virtual void setTriViewColorDispType(int mytype)  = 0;
	
	virtual void * getCustomStructPointer() const = 0; 
	virtual void setCustomStructPointer(void *a) = 0;
	
	virtual V3DLONG getValidZslice() const = 0;
	virtual void setValidZslice(V3DLONG curslice) = 0;
	
	virtual V3DLONG getPreValidZslice() const = 0;
	virtual void setPreValidZslice(V3DLONG preslice) = 0;
	
	virtual void trigger(V3DLONG curslice) = 0;

};

#endif /* __BASIC_TRIVIEW_H__ */
