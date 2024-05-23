#ifndef Constans_h
#define Constans_h
#endif

#define heightOfBullet 3 // TODO rename to something that makes sense 
#define widthOfBullet 1
#define sideOfMonster 5
#define gridYLimit 80
#define gridXLimit 80
#define moveDistanceForMonsters 1
#define moveDistanceForShip 2
#define intervalForInvulnerability 5000UL
#define intervalForSpeedingUp 30000UL
#define intervalForMovingBullets 50UL
#define intervalForMovingShip 150UL
#define shipWidth 7
#define shipHeight 3
#define spaceShipY 20
#define spaceShipX 36
#define startPositionX (gridXLimit+columnGap-((sideOfMonster+columnGap)*columns))/2
#define startPositionY 80
#define moveDistanceForMonsters 1
#define columnGap 4
#define rowGap 2
#define rows 3
#define columns 5
#define maxRows ((gridYLimit - spaceShipY) / (sideOfMonster + rowGap))
#define maxColumns ((gridXLimit + columnGap) / (sideOfMonster + columnGap))
#define speedDecerease 100UL
#define originalSpeed 800UL

#if rows > maxRows
#error tooManyRows Rows must be less than or equal to maxRows
#endif

#if columns > maxColumns
#error Way too many columns. Max value is 9
#endif

#if spaceShipX > gridXLimit - shipWidth || spaceShipX < 0
#error Ship is out of bounds horizontally
#endif

#if spaceShipY > gridYLimit || spaceShipY < shipHeight
#error Ship is out of bonds vertically; cannot be higher than monsters
#endif




