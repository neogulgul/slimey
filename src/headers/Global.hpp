#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>

#define LevelVector std::vector<std::vector<sf::Vector3i>>

#define emptyTile    sf::Vector3i(0, 0, 0)
#define spawnTile    sf::Vector3i(4, 0, 3)
#define exitTile     sf::Vector3i(4, 0, 4)
#define sawbladeTile sf::Vector3i(4, 1, 0)
#define turretUp     sf::Vector3i(4, 1, 1)
#define turretDown   sf::Vector3i(4, 1, 2)
#define turretLeft   sf::Vector3i(4, 1, 3)
#define turretRight  sf::Vector3i(4, 1, 4)
#define transparentSpawnTile sf::Vector3i(5, 0, 3)

namespace fs = std::filesystem;

const          int tilesize        = 16;
const unsigned int horizontalTiles = 16;
const unsigned int verticalTiles   = 16;

const unsigned int viewWidth       = tilesize * horizontalTiles;
const unsigned int viewHeight      = tilesize * verticalTiles;
const unsigned int windowViewScale = 3;
const unsigned int windowWidth     = viewWidth  * windowViewScale;
const unsigned int windowHeight    = viewHeight * windowViewScale;

const unsigned int targetFPS = 60;

const unsigned int sawbladeFrameCount    = 8;
const unsigned int sawbladeFrameDuration = 1;

const float GRAVITY = 0.16;

enum Direction { Up, Down, Left, Right };

enum MouseWheel { Forward = 1, Backward = -1 };

enum Alignment { Start, Center, End };

enum Tileset { Null, Normal, Ice, Bounce, Other };

enum State
{
	SplashScreen,
	ExitScreen,
	MainMenu,
	OptionsScreen,
	LevelEditor,
	StoryLevels,
	CustomLevels,
	LevelPlay,
	LevelClear
};

extern sf::Color               background;
extern sf::Color               foreground;
extern sf::Color inactiveButtonBackground;
extern sf::Color   activeButtonBackground;
extern sf::Color   lockedButtonBackground;
extern sf::Color inactiveButtonForeground;
extern sf::Color   activeButtonForeground;
extern sf::Color   lockedButtonForeground;
extern sf::Color               pauseColor;
// level
extern sf::Color collisionColor;
// editor
extern sf::Color     levelOutlineColor;
extern sf::Color levelCheckerEvenColor;
extern sf::Color  levelCheckerOddColor;
extern sf::Color   levelCrosshairColor;
extern sf::Color  levelRestrictedColor;
extern sf::Color            placeColor;
extern sf::Color            eraseColor;
extern sf::Color       selectionTilesetBackgroundColor;
extern sf::Color          selectionTilesetOutlineColor;
extern sf::Color selectionTilesetSelectionOutlineColor;

void toggle(bool &boolean);
std::string boolString(bool boolean);
bool pressing(sf::Keyboard::Key key);
bool pressing(sf::Mouse::Button button);
void handlePress(bool pressing, bool &press, bool &pressed);
void createSavedataFile(std::string path);
void createSavedataFolder(std::string path);
sf::Vector2f relativeViewPosition(sf::View* view, sf::Vector2f position);
sf::Vector2f align(sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position);
LevelVector getStoryLevelVector(int index);
LevelVector getCustomLevelVector(std::string levelName);

                                                                                                                                                                                                        const char 兄貴[] = "00000OOOOOOOOO00O0000OOOxoc:cc;,,,:cc:;:lolldOkxddxkxookkxocc:::;'.';codddxOOkkOOOO00000OOOkkkxxkkkO\n00OOOOOOOOOO000OO0Okxxxxocc:,,;;lddllcc:::lkKKkdox00xlkOxl:,;:;,',,..'',;;cdxkOOO00000OO0OOkkkkOOOOO\n0OOO00OOOkO00OkkOOkkOOOOxc;'',cx0xl:;;;,';dXOol:,lxdlldo,''.,,,'..'',,,::;:::coO000OOOOOOkkkkkOOOkxd\n0000OOO0OOOOOOkOkdkOOxol:,..''l0d:;'..'..,dx:;'..,::;;;'............';ccclc;'',lxO0OOOOOOOO000Okxxxx\n0000OO00OkkOkkxodxkOOdl:.....'ox;... ....';,...  .'''...   .........';::::::;,,';d0000OOO000Okxxkkkk\n0O00OOO0OkOkxkdd0K0KKKx:......:c'..    .......   .......   .. .......','..',,',,;lOK0000000OxxOO00O0\nKK000OO00Oxox00KNNXNNXx;'.. ...'...    ........   ... .           ..............':dO00OO0OkkO0000000\nKKKK0000kxddx0KK000XKOo;..    .....     .......    ..                   ..........:x0KK00OkkOOO00000\nKKK00000xooodkKKkdodxdl:'...   .................                       ...      ...:kK0O0000kOO00OkO\nKK00000Oxxdl:cxKKkdlllc;'...     ...............................        ..        ..:xOOO00OOO0OkkkO\nOOOO00Okkxoc;;;:oddoolc;,...     .....','..''''''''''...'''...'......              .,oO00000OOkxdkO0\nkOOO0Oxxxoc;,,'''';:cc:;'.........''',;;;;;;;:;;;;;;;;,,,,,,,,,,,'''........        .:xOOOOOkkkxxkOO\nkO000kdool:;;,..''',,;;;,''.'...';:;;cccclolllcllllcc:::;;;;;;;;,,,,''.......        .lOOkxxkOOkOOOO\nO00Oxdooolllc:;;::::cllllccc::;;;cc:lxdllodoooooodddddooolc:;;;;;;;,,,''.....        .:xkkO00O0OkOOO\nOOOkxdoodooooccclooddxk0Oxxxxolccllcx0xodxdxOOOkkkkOOOkkxdolc::;;;;;;,,,'......      .;x0K0OOOkkxO0O\n00kxdoollcccc:clxxdxxk0K000kxoloooldOkoxkxk0K0Okkkkkkkkkxxddlc::;;;;;;;;,''.....      'x00000kkxdkOO\n00OdooollcllllokOkkkOOOOKXXKOxxxdooxxodkkkKKK0OxxxkkOkkkxddolc:::::;;;;;;,,'.......   .oO00OkkOOkkkk\n00OkxkkxxollooxkOOOOOOOOOOOkxdxxdddxkkkxk0KKK0Okxkkkkkkxxdollccc::::;;;;;;,'...........;k00OOkkOkkOk\nOOkxxkxxollooxkkOOOOOO0KKOkdldxxxkOOOOkO000KKKOkxxxxxxxxxdoollcc:::::;;;;;;;,'........ .d000OkOOkOOk\nOOkoccccclddxxxxOOO0OO00Oxddxxxk000000000000K0Okkxdddxxkkxdooolcc::::::;;;;;;,''........;k00OOO00OOO\n00kdl:,;:loooodxkO00OOOOxdxO0O00KKXKKK000000000OOxxxxkkOkkxxddoolcc::::::::::;;,'........lOOkO00OO00\n0OOOx;',;;::clodxO0OkkkxxkO0000KKKKKKKK000000KK00kkkkkOOOkkxxddoolcccc:::::::::;;,'..... ,x0000OOOOO\n00OOOl'',,,;;cldkOOkO0OOO0000KKKKXXXXXXK00000KKK0Okkkkkkkxxdooollloooolc:::::::::;,'......o000000OO0\n00000d;''',,;:cok0OO0000000KKKKXXNNXXXXK00OO00000OOkxxddddoolllccllllccc::::::::::;,......;oxkkOOOOO\n0OO00kc,'''',:cdkO00000KKKKKKXXXXXXXXXK00OkkkOO00OOkdollllc::;,,,,,'','''',;;;;:::;,'.....;::lxOOOkk\nO0KOkxo:,''',:cldk0000KKKKKKXXXXXXXKK00OOkkxxkkkkkkxoc;,,,'..........'''',,,,;;;:::;'....'::,;lxxkOO\nOOkxdxddl'',,::cokO000KKKKKXXXXXXKKK0Okxxxxxddddddddl:;,''.......',;;;:;;;;;;;;::::;,....;cc;,;ldk00\n00OOOOOOd,',;:::oxO00KKKXXXXXKKK0Okxollc:cclllllolllcc:;;;;;;,,,;;;;;;;:::::;:::::::;'...:lll:,;oO00\nOO00OO00kc'',:::oxO00KKXXXXK0kxdoc:,''''',;;::cccccccclccc:;,''........'',;;;;;::::::;,..':cllc;ckKK\nOOO0000KKx;.',;:lk000KKK0Oxol:;,'....'',,,,,;:cccccccccclc;'..... ....',,,,,,,;;;:::::;'..',;:c::dOO\nOOO0K0O00Ox:.',;oOKKKK0Oxoc:;,''''',,,,,,,;;::cldddolcccc;,'....',;;:::::;;;;::::::::::;'.',',::;cdx\n000KK0000O0k:'';lOKKKKOdlc:;;;;;;;,'''...'',;:cdkkkxolc:::;,'''',;;::;;;;;;;;:::cc::::::,.,,'':c:cdx\n00KK000000OOko;,:x0KKKOdc:clc:;,...........';:o0XK0Okdl::::;;,,,,,,,,,,,,,,;;::::ccc:::;,'',';cc:ldd\n0KKK0OO00K00KOo;;lOKKKOo:ldl;'.......''...',:oOXNXK0Oxoc:::::::::;;;;;;;;;;;:::cccccccc:,'.,;cllcokx\nK00K0O0O0OkO0xll:;dKK0kooddc:::::;:;;;,'',;cdOKXNXK0Oxoc::::::::;;:::::::::::::::cccccc:;',:lllcldkk\nK00K00000OO0Kxldklo0KK0kkkkkOOxoc::;;,;cloxO000KXXXKOkol::::ccccc::::::::::ccccccccccccc:;;:clccokOO\n0OO000O00O00Kkok0xdOKKK00K000Okkxkkxxk0KKXXXKKKKXNNX0Oxolcc:ccccc::::::ccccllllllccccccc:;,;cllok00O\nOO00KKOkOO000OxkOdokKKKKKK0KKKKXNNNNNNNNNNNXXXKKXNWNX0Okxdocloddl:::::::ccclloolllcccccc::;;:ldkOOkx\nKOO0000OOOOOkOkkkdloO000000KKKKKXXXNNXXXK0OO0XXXXNWWWX0Okxocoxxdlc:c:;;;:::clllllcllccc::::;;lxOOkxx\nO00OO00O00Okk00OOOdodO00OO000000KKKXKK0OxolxKXXXKXXNXKOkxolcclollc::c::;;;;:cclllllcccc::::;;codxOOk\nO00OO000000000000KK0OkO0OOOO00000000OOxol::xKXKK0000Oxdolllc::c::;;;;:;;;;,,;:clllc::cc::::;;:odk00k\nO00OkO00000OO0OOKXNNXkk00OOO00000OOkxocc:;;ck0Okkxxdolllllc:;;;,,,,;;,;;;;;;;;;:cllc::::::::;;cx00kk\nO0KOk00OOOkkkkkxkKNXK0kO0OO00OOOkkxdoc::;,;:cddl;,,;::ccccc::;;;,,,,;;;:::::::;;:clc:;:::;;::;;:oodO\n0KKOkOkkkxkkxxkxxOKXKK0OOOO00OOkxdolc::;,,;;cdko:,',,,,;;;;;,;;;;;;:::c::::;;::;;:cl:;::::;::;,,,,cd\n00Oxxkkxxxxkxkxddkk0XXX0OOOOOkxxdlccc:;;,,,;lxkxdlc:,,,,,,',,;;:::ccc::;,'.',,;;;:clc;:::;::;;,,,,,,\n0OxkOkkxxxxxkkddxkxkkOOkxkOOOxdddolc::;,,,,:dkkkkxxdl::;;;,;:::::cclll;. ..',,,;;:clc:::;;;;;;;,,,,,\nkxO0kxxxxxxxxxddxxxkxxxdxkOOkdloxxoc::;,,;;:oddddooolc::::::cllcldoxxc....,;;;;;;:ccc:;:;;,;;;;,,,,,\nkk0Oxxxxxxxxxxdddxkkxxxxkkkkkxllxkdl:;,;;;;;::;;::c:clcclooloxxdxxol:...';;::;;,;;:::;;;;,,,,;;,,,,,\nOkOxdxxxxxxxxxddxxxxxddxkkxkkxolokxoc:;;;;,'.. ..;clldxdxkkxxxxdol;'.'',;::::;,,,;:c:,,;,,,,,;;;,,,,\nkkxddxkkxxxxkxdxxxxxxddxkkkkxxxooxkdlc:;;,,,,'.....,:clllddollcc:;'',,;::::::;;;,;::;,,,,,,;;;;;;,,,\nxxxxxxkkxxxddxdxxxdxddxxxkOOxxddddkxol::;,,;;:cloddddddddoolc:;;;;::::cc::::;;,;;;;;,,,,,,,;;;;;;;,,\nxxkkkxkkxxxdddxxxxdxxdxxxxkOOxddddxxdlc:;;;;coxO00KKKXXKK0OOkdllccccccccc:::;,,,;;;,,,,',,;;;;;;;;;,\nxxkkkxkkxxxxxxxdxxxxddxxdddxkkxdoooodoc;;;;:oxkkOOOOkkkkkxxdollcc:::::::::;;,,,,,,,,,'',;;;;;;;;;;;;\nxxkOkkkkkkxxxxddxxdddxxddxkO0KKKkdllccc:;;,:ldxkkkkkxxddolcc:;;;;;;;;;::;;;,,,,,,,,,'',;;;;;;;;;;;;;\nkkkOkkO0OkxxddxxxxdddkkO0KXNNNNNNKkl:::::;;:codxxxkxddollc:;;,,,,,,;;;;;;,,,,,,,,,,'',;;;;;:;;::;;;;\nkO0OkkOOkxkxddxkxxxk0KXNNNNNNNNNNNNXOoc;:::cclodxkkkxddol:;,,''',',,,,,,;;;;;,,;,'',,,;;;;;;::::;;;;\nkOO0OxkOkxxxxxxkO0KXNNNNNNNNNNNNNNWWWN0xc:cccoddxxxkkkkkxdollc::::ccc:;;;;;;;,,,'',,,;;;;;;;;;;:::;;\nkkkOkkxkkxxkkO0KXNNNNNNNNNNNNNNNNNNNNNNN0dlccloodxxxkkO0OOOOkxkxddxdoc:;;;,,;,,',,,,,;;;;;;;;;;;;;;;\nxdxxk0OkOOO0KXXXXXNNNNNNNNNNNNNNNNNNNNXXXKkdlccldddxkkkOkOOOxddolllc:::;;;;,,,,,,,;;,;;;;;;;;;;;;;;;\nOOO0KKKKKXXXXXXXXXXXXXNNXXXXNNXXNNXXXXXXKK0Oxdlcloodddddddddollc::;;;;;;,,,,,,,,,,,,,;;;;;;;;;;;;;;;\nKKKKKKXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXKKK00OOxxolcccccllccc:::;;;;;;;,,,,,,;;;,,;;,;;;;;;;;;;;;;;;;;\n0000KKXXXXXXXXXXXXXKKKKKKKKKKKKKXXXKKKKK00OOOOkddoolcc::;;,,,,,;;,;,,,,;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n000000KXXXXXXXXXXXXXKKKKKKK00000KKXXXKKKK000000kddoollc:;;;,,,,;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,\n000000KXXXXXXXXXXXKKXKKKKK00OOOO0KXXXXKKK000KKX0kddollc:::;;;;:::::;;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,\nK0000KKXXXXXXXXXXXKKKKKK00OOOOkO0KXXXXXKK00KKXNX0xxdocc::::;;;:;::;;;;;;;;;;:;;;;;;;;;;;;;;;;,,,,,,,\n000KKKXXXXXXXXXXKKKK0000OOkkkxkO0KXNXXXKK000KXNNKOkxdoc:::::;;:;;;;;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,\nKKKKKKKKKXXXXXXKKKK000OOOkkxxxkO0KXNXXXK00000KXXX0Okxdoc:::::::;;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,\nKKKXKKKKKKXXXXKK00000OOkkxxxxxkOKXXNNXXKK000O0KXXK00kxdoc::::::;;;;;;;;;;;;;;;;;;;;;;,,,,,,,'',,''''\nKKKKKKKKKKKKKKK0000OOkkkxxxxxxk0XXNNNNXXKK00000KKXKKOkxol::::::;;;;;;;;;;;;;;;;;;;,,,,,,',,,''''''..\nKKKKKKKKKKKKKKK000OOkkkkkxxxxxOKXXNNNXXXKKKK0KKKKKKK0Oxdlc::::;;;;;;;;;;;;;;;;;;;,,,,,,,,,,''''.....\nKKKKKKKKKKKKK0000OOkkxxxxxxxxk0KXXNXXXXXKKKKKKKKKKKKK0Oxdlc:::;;;;;;;;,,,,;;;;;;,,,,,,,'''''........\nKKKKKKKKKKK00000OOkkkxxkxxkkkO0KXXXXXXXXKKKKKKKKKKKKK00Okdolc:;;;;;;;;;;;,,;;;;;,,,,,,,'''''''''''''\nKKKKKKKKK0000O0OOkkkxxxkxxkOO0KKXXXXXKKKKKKKKKKKKKKK000OOkxdlc:;;;;;;;,,,,;;;;;;,,,,,,,,'''',,''''',\nKKKKKKKK0000OOOOOkkkkkkkkOOO000KKKKKKKKKKKKKKKKK00000000OOkxolc:::;;;;,,;;;;;;;;;,,,,,,,,,,,,,,,,,,,\nKKKKKKKK000OOOOOOkkkkkkOOO00000KKKKKKKKKKK0KKKK000000000OOkxolcc:::;;;;;;;;;;;;;;;;,,,,,,,,;,,,,,,,,";
