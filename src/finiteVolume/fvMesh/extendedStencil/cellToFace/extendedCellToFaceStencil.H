/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    Foam::extendedCellToFaceStencil

Description
    Calculates/constains the extended cell-to-face stencil.

    The stencil is a list of indices into either cells or boundary faces
    in a compact way. (element 0 is owner, 1 is neighbour). The index numbering
    is
    - cells first
    - then all (non-empty patch) boundary faces

    When used in evaluation is a two stage process:
    - collect the data (cell data and non-empty boundaries) into a
    single field
    - (parallel) distribute the field
    - sum the weights*field.

SourceFiles
    extendedCellToFaceStencil.C
    extendedCellToFaceStencilTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef extendedCellToFaceStencil_H
#define extendedCellToFaceStencil_H

#include "mapDistribute.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class globalIndex;

/*---------------------------------------------------------------------------*\
                 Class extendedCellToFaceStencil Declaration
\*---------------------------------------------------------------------------*/

class extendedCellToFaceStencil
{
protected:

    // Protected data

        //- Reference to mesh
        const polyMesh& mesh_;


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        extendedCellToFaceStencil(const extendedCellToFaceStencil&);

        //- Disallow default bitwise assignment
        void operator=(const extendedCellToFaceStencil&);


protected:

        //- Write some statistics about stencil
        static void writeStencilStats
        (
            Ostream& os,
            const labelListList& stencil,
            const mapDistribute& map
        );


public:

    // Declare name of the class and its debug switch
    ClassName("extendedCellToFaceStencil");


    // Constructors

        //- Construct from mesh
        explicit extendedCellToFaceStencil(const polyMesh&);


    // Member Functions

        //- Calculate distribute map
        static autoPtr<mapDistribute> calcDistributeMap
        (
            const polyMesh& mesh,
            const globalIndex& globalNumbering,
            labelListList& faceStencil
        );

        //- Use map to get the data into stencil order
        template<class T>
        static void collectData
        (
            const mapDistribute& map,
            const labelListList& stencil,
            const GeometricField<T, fvPatchField, volMesh>& fld,
            List<List<T> >& stencilFld
        );

        //- Sum vol field contributions to create face values
        template<class Type>
        static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        weightedSum
        (
            const mapDistribute& map,
            const labelListList& stencil,
            const GeometricField<Type, fvPatchField, volMesh>& fld,
            const List<List<scalar> >& stencilWeights
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "extendedCellToFaceStencilTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
