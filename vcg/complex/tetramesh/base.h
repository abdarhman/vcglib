/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
  History

$Log: not supported by cvs2svn $
Revision 1.3  2004/05/06 10:57:49  pietroni
changed names to topology functions

Revision 1.2  2004/04/28 11:37:14  pietroni
*** empty log message ***

Revision 1.1  2004/04/20 12:41:39  pietroni
*** empty log message ***

Revision 1.1  2004/04/15 08:54:20  pietroni
*** empty log message ***


***************************************************************************/


#pragma warning( disable : 4804 )
#pragma once

#ifndef __VCG_TETRAMESH
#define __VCG_TETRAMESH
#include <vcg\space\box3.h>


namespace vcg {
namespace tetra {
 /** \addtogroup trimesh */
/*@{*/

  /**  Class TetraMesh.
 This is class for definition of a mesh.
		@param STL_VERT_CONT (Template Parameter) Specifies the type of the vertices container any the vertex type.
		@param STL_FACE_CONT (Template Parameter) Specifies the type of the faces container any the face type.
 */


template < class STL_VERT_CONT ,class STL_TETRA_CONT >
class Tetramesh{
	public:

/***********************************************/
/** @name Tetramesh Type Definitions **/
//@{
  
  /// The mesh type
	typedef Tetramesh<STL_VERT_CONT,STL_TETRA_CONT> TetraMeshType;

	/// The vertex container
	typedef STL_VERT_CONT VertexContainer;

	/// The tethaedhron container
	typedef STL_TETRA_CONT TetraContainer;

	/// The vertex type 
	typedef typename STL_VERT_CONT::value_type VertexType;
	
	/// The tetrahedron type 
	typedef typename STL_TETRA_CONT::value_type TetraType;

	/// The type of vertex iterator
	typedef typename STL_VERT_CONT::iterator VertexIterator;

	/// The type of tetra iterator
	typedef typename STL_TETRA_CONT::iterator TetraIterator;

	/// The type of constant vertex iterator
	typedef typename STL_VERT_CONT::const_iterator const_VertexIterator;

	/// The type of constant face iterator
	typedef typename STL_TETRA_CONT::const_iterator const_TetraIterator;

	/// The vertex pointer type
	typedef VertexType * VertexPointer;

	/// The tetra pointer type
	typedef VertexType * TetraPointer;

	/// The type of the constant vertex pointer
	typedef const VertexType * const_VertexPointer;

	/// The type of the constant tetrahedron pointer
	typedef const VertexType * const_TetraPointer;

	typedef typename VertexType::ScalarType ScalarType;
//@}

/***********************************************/
/** @Common Attributes of a tetrahedral mesh **/
//@{

	///temporary mark for decimation
	int IMark;

	/// Set of vertices 
	STL_VERT_CONT vert;

	/// Real number of vertices
	int vn;

	/// Set of tetrahedron
	STL_TETRA_CONT tetra;

	/// Real number of tetrahedron
	int tn;
  
  /// Real number of edges
	int en;

  ///Boundingbox della mesh
  Box3<ScalarType> bbox;
//@}
 
/***********************************************/
/** @Default Functions **/
//@{

	/// Default constructor
	Tetramesh()
	{   
		tn = vn = en = 0;
	}
	
	Tetramesh(VertexContainer v,TetraContainer t)
	{
		this->vert=v;
		this->tetra=t;
		vn=v.size();
		tn=t.size();
	}

	inline int MemUsed() const
	{
		return sizeof(TMTYPE)+sizeof(VertexType)*vert.size()+sizeof(tetrahedron)*tetra.size()+sizeof(edge)*edges.size();
	}
//@}

/***********************************************/
/** @Functions used to retrieve informations**/
//@{
 /// Reflection functions that speak about vertex and face properties.
static bool HasPerVertexNormal()  { return VertexType::HasNormal() ; }
static bool HasPerVertexColor()   { return VertexType::HasColor()  ; }
static bool HasPerVertexMark()    { return VertexType::HasMark()   ; }
static bool HasPerVertexQuality() { return VertexType::HasQuality(); }
static bool HasPerVertexTexture() { return VertexType::HasTexture(); }

static bool HasPerTetraNormal()    { return TetraType::HasTetraNormal()  ; }
static bool HasPerTetraMark()      { return TetraType::HasTetraMark()   ; }
static bool HasPerTetraQuality()   { return TetraType::HasTetraQuality(); }

static bool HasTTTopology()       { return TetraType::HasTTAdjacency();  }
static bool HasVTTopology()       { return TetraType::HasVTAdjacency(); }
static bool HasTopology()         { return HasTTTopology() || HasVTTopology(); }

/***********************************************/

/** @Functions used for handle the temporany mark of a tetrahedron used in decimation**/
//@{

///Increase the current mark.
	void UnMarkAll()
	{	
		++IMark;
	}

///Mark the vertex with current value
	void Mark(VertexType *v)
	{
		 v->IMark()=IMark;
	}

///Initialize the mark of all vertices
	void InitIMark()
	{
	VertexIterator vi;
	IMark=0;
	for(vi=vert.begin();vi!=vert.end();vi++)
	{
		(*vi).InitIMark();
	}
	}

//@}


void LoadTs(char * filename, double meshscale )
{	
	int nvertex;
	int ntetra;
	float x;
	float y;
	float z;
	int tp0;
	int tp1;
	int tp2;
	int tp3;
	float mass;
	FILE *f;
	Tetramesh::VertexType p1;
	f = fopen(filename,"r");
	if(f == NULL ) 
     printf( "The file was not opened\n" );
   else
   {
		fscanf(f, "%i", &nvertex );
		fscanf(f, "%i", &ntetra );
		int j;
		for (j=0;j<nvertex;j++)
		{
			fscanf(f, "%f", &x );
			fscanf(f, "%f", &y );
			fscanf(f, "%f", &z );
		  //fscanf(f, "%f", &mass );
      p1.ClearFlags();
      p1.P()=Point3d(x*meshscale, y*meshscale ,z*meshscale );
			vert.push_back(p1);
		}
		tetra.reserve(ntetra*10);
    vert.reserve(nvertex*10);
		for (j=0;j<ntetra;j++)
		{
			fscanf(f, "%i", &tp0 );
			fscanf(f, "%i", &tp1 );
			fscanf(f, "%i", &tp2 );
			fscanf(f, "%i", &tp3 );
			
			Tetramesh::TetraType  newTetra;
			tetra.push_back(newTetra);
			tetra.back().Init(&vert[tp0],&vert[tp1],&vert[tp2],&vert[tp3]); 
		}
   }
}

};//End class

/*@}*/


};//end namespace
};//end namespace
#endif