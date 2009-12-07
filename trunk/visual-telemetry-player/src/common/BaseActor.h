/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 *  BaseActor.h
 *  TWS
 *
 *  Created by Filipe Varela on 08/11/05.
 *  Copyright 2008 Filipe Varela. All rights reserved.
 *
 */

#ifndef _TWS_BASEACTOR
#define _TWS_BASEACTOR

#include "Node.h"

namespace TWS {
    class BaseActor : public Node {
    protected:
        // holds maximum and current health
        short m_maxHealth;
        short m_currentHealth;
        
        // signals that unit status has changed and
        // needs to be sync'd to network for other players
        bool m_needsSync;
    public:
        BaseActor();
        ~BaseActor();
        
        short getHealth() { return m_currentHealth; }
    };
}

#endif
