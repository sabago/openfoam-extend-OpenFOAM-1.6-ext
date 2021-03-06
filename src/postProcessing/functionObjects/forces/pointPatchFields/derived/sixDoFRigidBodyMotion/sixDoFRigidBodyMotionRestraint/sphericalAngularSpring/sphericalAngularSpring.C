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

\*---------------------------------------------------------------------------*/

#include "sphericalAngularSpring.H"
#include "addToRunTimeSelectionTable.H"
#include "sixDoFRigidBodyMotion.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace sixDoFRigidBodyMotionRestraints
{
    defineTypeNameAndDebug(sphericalAngularSpring, 0);
    addToRunTimeSelectionTable
    (
        sixDoFRigidBodyMotionRestraint,
        sphericalAngularSpring,
        dictionary
    );
};
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::
sphericalAngularSpring
(
    const dictionary& sDoFRBMRDict
)
:
    sixDoFRigidBodyMotionRestraint(sDoFRBMRDict),
    refQ_(),
    stiffness_(),
    damping_()
{
    read(sDoFRBMRDict);
}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

Foam::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::
~sphericalAngularSpring()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void
Foam::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::restrain
(
    const sixDoFRigidBodyMotion& motion,
    vector& restraintPosition,
    vector& restraintForce,
    vector& restraintMoment
) const
{
    restraintMoment = vector::zero;

    for (direction cmpt=0; cmpt<vector::nComponents; cmpt++)
    {
        vector axis = vector::zero;

        axis[cmpt] = 1;

        vector refDir = vector::zero;

        refDir[(cmpt + 1) % 3] = 1;

        vector newDir = motion.orientation() & refDir;

        axis = (refQ_ & axis);

        refDir = (refQ_ & refDir);

        newDir -= (axis & newDir)*axis;

        restraintMoment += -stiffness_*(refDir ^ newDir);
    }

    restraintMoment += -damping_*motion.omega();

    restraintForce = vector::zero;

    // Not needed to be altered as restraintForce is zero, but set to
    // centreOfMass to be sure of no spurious moment
    restraintPosition = motion.centreOfMass();

    if (motion.report())
    {
        Info<< " force " << restraintForce
            << " moment " << restraintMoment
            << endl;
    }
}


bool Foam::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::read
(
    const dictionary& sDoFRBMRDict
)
{
    sixDoFRigidBodyMotionRestraint::read(sDoFRBMRDict);

    refQ_ = sDoFRBMRCoeffs_.lookupOrDefault<tensor>("referenceOrientation", I);

    if (mag(mag(refQ_) - sqrt(3.0)) > 1e-9)
    {
        FatalErrorIn
        (
            "Foam::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::"
            "read"
            "("
                "const dictionary& sDoFRBMRDict"
            ")"
        )
            << "referenceOrientation " << refQ_ << " is not a rotation tensor. "
            << "mag(referenceOrientation) - sqrt(3) = "
            << mag(refQ_) - sqrt(3.0) << nl
            << exit(FatalError);
    }

    sDoFRBMRCoeffs_.lookup("stiffness") >> stiffness_;

    sDoFRBMRCoeffs_.lookup("damping") >> damping_;

    return true;
}


void Foam::sixDoFRigidBodyMotionRestraints::sphericalAngularSpring::write
(
    Ostream& os
) const
{
    os.writeKeyword("referenceOrientation")
        << refQ_ << token::END_STATEMENT << nl;

    os.writeKeyword("stiffness") << stiffness_ << token::END_STATEMENT << nl;

    os.writeKeyword("damping") << damping_ << token::END_STATEMENT << nl;
}


// ************************************************************************* //
