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

#include "wordRe.H"
#include "IOstreams.H"
#include "InfoProxy.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::wordRe::wordRe(Istream& is)
:
    word(),
    re_(NULL)
{
    is >> *this;
}


Foam::Istream& Foam::operator>>(Istream& is, wordRe& w)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isWord())
    {
        w = t.wordToken();
    }
    else if (t.isString())
    {
        // Auto-tests for regular expression
        w = t.stringToken();
    }
    else
    {
        is.setBad();
        FatalIOErrorIn("operator>>(Istream&, wordRe&)", is)
            << "wrong token type - expected word or string found "
            << t.info()
            << exit(FatalIOError);

        return is;
    }

    // Check state of IOstream
    is.check("Istream& operator>>(Istream&, wordRe&)");

    return is;
}


Foam::Ostream& Foam::operator<<(Ostream& os, const wordRe& w)
{
    os.writeQuoted(w, w.isPattern());
    os.check("Ostream& operator<<(Ostream&, const wordRe&)");
    return os;
}


Foam::Ostream& Foam::wordRe::info(Ostream& os) const
{
    if (isPattern())
    {
        os  << "wordRe(regex) " << *this;
    }
    else
    {
        os  << "wordRe(plain) \"" << *this << '"';
    }
    os.flush();

    return os;
}


// ************************************************************************* //
