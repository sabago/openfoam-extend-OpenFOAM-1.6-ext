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
    Foam::instant

Description
    An instant of time. Contains the time value and name.

SourceFiles
    instant.C

\*---------------------------------------------------------------------------*/

#ifndef instant_H
#define instant_H

#include "word.H"
#include "scalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

class instant;

// Friend Operators

bool operator==(const instant&, const instant&);
bool operator!=(const instant&, const instant&);

// IOstream Operators

Istream& operator>>(Istream&, instant&);
Ostream& operator<<(Ostream&, const instant&);


/*---------------------------------------------------------------------------*\
                           Class instant Declaration
\*---------------------------------------------------------------------------*/

class instant
{
    // Private data

        scalar value_;
        word name_;

public:

    // Public classes

        //- Less function class used in sorting instants
        class less
        {
        public:

            bool operator()(const instant& a, const instant& b) const
            {
                return a.value() < b.value();
            }
        };


    // Static data members

        static const char* const typeName;


    // Constructors

        //- Construct null
        instant();

        //- Construct from components
        instant(const scalar, const word&);

        //- Construct from time value
        instant(const scalar);

        //- Construct from word
        instant(const word&);


    // Member Functions

        // Access

            //- Value (const access)
            scalar value() const
            {
                return value_;
            }

            //- Value (non-const access)
            scalar& value()
            {
                return value_;
            }

            //- Name (const access)
            const word& name() const
            {
                return name_;
            }

            //- Name (non-const access)
            word& name()
            {
                return name_;
            }


    // Friend Operators

        friend bool operator==(const instant&, const instant&);
        friend bool operator!=(const instant&, const instant&);


    // IOstream Operators

        friend Istream& operator>>(Istream&, instant&);
        friend Ostream& operator<<(Ostream&, const instant&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
