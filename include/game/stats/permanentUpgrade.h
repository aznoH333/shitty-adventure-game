#ifndef PERMANENT_UPGRADE
#define PERMANENT_UPGRADE

#include "item.h"
#include "stat.h"


namespace PlayerStats {
    class PermanentItem : public Item {
        public:
        PermanentItem(int itemId, int itemSeed, StatName targetStat);
    };
}


#endif