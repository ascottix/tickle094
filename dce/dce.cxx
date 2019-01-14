#include "dce.h"

DBit DLine::Unconnected = 0;
DBit DLine::Power       = 1;
DBit DLine::Ground      = 0;
DBit DLine::High        = 1;
DBit DLine::Low         = 0;

unsigned DLine::numOfLines           = 0;
unsigned DLine::numOfLinesAssigned   = 0;
unsigned DLine::numOfLinesConnected  = 0;

unsigned DChip::numOfChips      = 0;
unsigned DChip::numOfChipLines  = 0;
