#ifndef Constans_h
#define Constans_h
#endif

#if rows > maxRows
#error tooManyRows Rows must be less than or equal to maxRows
#endif
#if columns > maxColumns
#error tooManyCol Columns must be less than or equal to maxColumns
#endif

#define heightOfBullet 3 // TODO rename to something that makes sense 
#define widthOfBullet 1
#define sideOfMonster 5
#define gridYLimit 80
#define gridXLimit 80
#define moveDistanceForMonsters 1
#define moveDistanceForShip 2
#define intervalForInvulnerability 5000UL
#define intervalForSpeedingUp 1000UL
#define intervalForMovingBullets 50UL
#define shipWidth 7
#define shipHeight 3
#define spaceShipY 10 
#define startPositionX (gridXLimit+columnGap-((sideOfMonster+columnGap)*columns))/2
#define startPositionY 80
#define moveDistanceForMonsters 1
#define columnGap 4
#define rowGap 2
#define maxRows ((gridYLimit - spaceShipY) / (sideOfMonster + rowGap))
#define maxColumns (gridXLimit / (sideOfMonster + columnGap))
#define rows 5
#define columns 7



