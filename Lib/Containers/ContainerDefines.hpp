#pragma once

#include "Defines.hpp"

#define UPPER_CHAR		0x01
#define LOWER_CHAR		0x02
#define DIGIT_CHAR		0x04
#define SPACE_CHAR		0x08
#define PUNCT_CHAR		0x10
#define CONTROL_CHAR	0x20
#define HEX_CHAR		0x40

#define ALPHA_CHAR		UPPER_CHAR | LOWER_CHAR
#define ALPHANUM_CHAR	ALPHA_CHAR | DIGIT_CHAR

static inline constexpr C8 TYPE_LOOKUP[]{
	0,							// -1 EOF
	CONTROL_CHAR,				// 00 (NUL)
	CONTROL_CHAR,				// 01 (SOH)
	CONTROL_CHAR,				// 02 (STX)
	CONTROL_CHAR,				// 03 (ETX)
	CONTROL_CHAR,				// 04 (EOT)
	CONTROL_CHAR,				// 05 (ENQ)
	CONTROL_CHAR,				// 06 (ACK)
	CONTROL_CHAR,				// 07 (BEL)
	CONTROL_CHAR,				// 08 (BS)
	SPACE_CHAR | CONTROL_CHAR,	// 09 (HT)
	SPACE_CHAR | CONTROL_CHAR,	// 0A (LF)
	SPACE_CHAR | CONTROL_CHAR,	// 0B (VT)
	SPACE_CHAR | CONTROL_CHAR,	// 0C (FF)
	SPACE_CHAR | CONTROL_CHAR,	// 0D (CR)
	CONTROL_CHAR,				// 0E (SI)
	CONTROL_CHAR,				// 0F (SO)
	CONTROL_CHAR,				// 10 (DLE)
	CONTROL_CHAR,				// 11 (DC1)
	CONTROL_CHAR,				// 12 (DC2)
	CONTROL_CHAR,				// 13 (DC3)
	CONTROL_CHAR,				// 14 (DC4)
	CONTROL_CHAR,				// 15 (NAK)
	CONTROL_CHAR,				// 16 (SYN)
	CONTROL_CHAR,				// 17 (ETB)
	CONTROL_CHAR,				// 18 (CAN)
	CONTROL_CHAR,				// 19 (EM)
	CONTROL_CHAR,				// 1A (SUB)
	CONTROL_CHAR,				// 1B (ESC)
	CONTROL_CHAR,				// 1C (FS)
	CONTROL_CHAR,				// 1D (GS)
	CONTROL_CHAR,				// 1E (RS)
	CONTROL_CHAR,				// 1F (US)
	SPACE_CHAR,					// 20 SPACE
	PUNCT_CHAR,					// 21 !
	PUNCT_CHAR,					// 22 "
	PUNCT_CHAR,					// 23 #
	PUNCT_CHAR,					// 24 $
	PUNCT_CHAR,					// 25 %
	PUNCT_CHAR,					// 26 &
	PUNCT_CHAR,					// 27 '
	PUNCT_CHAR,					// 28 (
	PUNCT_CHAR,					// 29 )
	PUNCT_CHAR,					// 2A *
	PUNCT_CHAR,					// 2B +
	PUNCT_CHAR,					// 2C ,
	PUNCT_CHAR,					// 2D -
	PUNCT_CHAR,					// 2E .
	PUNCT_CHAR,					// 2F /
	DIGIT_CHAR | HEX_CHAR,		// 30 0
	DIGIT_CHAR | HEX_CHAR,		// 31 1
	DIGIT_CHAR | HEX_CHAR,		// 32 2
	DIGIT_CHAR | HEX_CHAR,		// 33 3
	DIGIT_CHAR | HEX_CHAR,		// 34 4
	DIGIT_CHAR | HEX_CHAR,		// 35 5
	DIGIT_CHAR | HEX_CHAR,		// 36 6
	DIGIT_CHAR | HEX_CHAR,		// 37 7
	DIGIT_CHAR | HEX_CHAR,		// 38 8
	DIGIT_CHAR | HEX_CHAR,		// 39 9
	PUNCT_CHAR,					// 3A :
	PUNCT_CHAR,					// 3B ;
	PUNCT_CHAR,					// 3C <
	PUNCT_CHAR,					// 3D =
	PUNCT_CHAR,					// 3E >
	PUNCT_CHAR,					// 3F ?
	PUNCT_CHAR,					// 40 @
	UPPER_CHAR + HEX_CHAR,		// 41 A
	UPPER_CHAR + HEX_CHAR,		// 42 B
	UPPER_CHAR + HEX_CHAR,		// 43 C
	UPPER_CHAR + HEX_CHAR,		// 44 D
	UPPER_CHAR + HEX_CHAR,		// 45 E
	UPPER_CHAR + HEX_CHAR,		// 46 F
	UPPER_CHAR,					// 47 G
	UPPER_CHAR,					// 48 H
	UPPER_CHAR,					// 49 I
	UPPER_CHAR,					// 4A J
	UPPER_CHAR,					// 4B K
	UPPER_CHAR,					// 4C L
	UPPER_CHAR,					// 4D M
	UPPER_CHAR,					// 4E N
	UPPER_CHAR,					// 4F O
	UPPER_CHAR,					// 50 P
	UPPER_CHAR,					// 51 Q
	UPPER_CHAR,					// 52 R
	UPPER_CHAR,					// 53 S
	UPPER_CHAR,					// 54 T
	UPPER_CHAR,					// 55 U
	UPPER_CHAR,					// 56 V
	UPPER_CHAR,					// 57 W
	UPPER_CHAR,					// 58 X
	UPPER_CHAR,					// 59 Y
	UPPER_CHAR,					// 5A Z
	PUNCT_CHAR,					// 5B [
	PUNCT_CHAR,					// 5C \ 
	PUNCT_CHAR,					// 5D ]
	PUNCT_CHAR,					// 5E ^
	PUNCT_CHAR,					// 5F _
	PUNCT_CHAR,					// 60 `
	LOWER_CHAR + HEX_CHAR,		// 61 a
	LOWER_CHAR + HEX_CHAR,		// 62 b
	LOWER_CHAR + HEX_CHAR,		// 63 c
	LOWER_CHAR + HEX_CHAR,		// 64 d
	LOWER_CHAR + HEX_CHAR,		// 65 e
	LOWER_CHAR + HEX_CHAR,		// 66 f
	LOWER_CHAR,					// 67 g
	LOWER_CHAR,					// 68 h
	LOWER_CHAR,					// 69 i
	LOWER_CHAR,					// 6A j
	LOWER_CHAR,					// 6B k
	LOWER_CHAR,					// 6C l
	LOWER_CHAR,					// 6D m
	LOWER_CHAR,					// 6E n
	LOWER_CHAR,					// 6F o
	LOWER_CHAR,					// 70 p
	LOWER_CHAR,					// 71 q
	LOWER_CHAR,					// 72 r
	LOWER_CHAR,					// 73 s
	LOWER_CHAR,					// 74 t
	LOWER_CHAR,					// 75 u
	LOWER_CHAR,					// 76 v
	LOWER_CHAR,					// 77 w
	LOWER_CHAR,					// 78 x
	LOWER_CHAR,					// 79 y
	LOWER_CHAR,					// 7A z
	PUNCT_CHAR,					// 7B {
	PUNCT_CHAR,					// 7C |
	PUNCT_CHAR,					// 7D }
	PUNCT_CHAR,					// 7E ~
	CONTROL_CHAR,				// 7F (DEL)
};

template<Character C>
struct StringLookup { };

template<>
struct StringLookup<char>
{
	static inline const char* TRUE_STR = "true";
	static inline const char* FALSE_STR = "false";
	static inline const char NULL_CHAR = '\0';
	static inline const char NEGATIVE_CHAR = '-';
	static inline const char POSITIVE_CHAR = '+';
	static inline const char DECIMAL_CHAR = '.';
	static inline const char ZERO_CHAR = '0';
	static inline const char BACK_SLASH = '\\';
	static inline const char FORWARD_SLASH = '/';
	static inline const char SPACE = ' ';
	static inline const char HTAB = '\t';
	static inline const char VTAB = '\v';
	static inline const char RETURN = '\r';
	static inline const char NEW_LINE = '\n';
	static inline const char FEED = '\f';
	static inline const char OPEN_BRACE = '{';
	static inline const char CLOSE_BRACE = '}';
	static inline const char FMT_HEX = 'h';
	static inline const char FMT_DEC = '.';

	static inline const char* DECIMAL_LOOKUP =
		"000001002003004005006007008009010011012013014015016017018019"
		"020021022023024025026027028029030031032033034035036037038039"
		"040041042043044045046047048049050051052053054055056057058059"
		"060061062063064065066067068069070071072073074075076077078079"
		"080081082083084085086087088089090091092093094095096097098099"
		"100101102103104105106107108109110111112113114115116117118119"
		"120121122123124125126127128129130131132133134135136137138139"
		"140141142143144145146147148149150151152153154155156157158159"
		"160161162163164165166167168169170171172173174175176177178179"
		"180181182183184185186187188189190191192193194195196197198199"
		"200201202203204205206207208209210211212213214215216217218219"
		"220221222223224225226227228229230231232233234235236237238239"
		"240241242243244245246247248249250251252253254255256257258259"
		"260261262263264265266267268269270271272273274275276277278279"
		"280281282283284285286287288289290291292293294295296297298299"
		"300301302303304305306307038309310311312313314315316317318319"
		"320321322323324325326327238329330331332333334335336337338339"
		"340341342343344345346347438349350351352353354355356357358359"
		"360361362363364365366367638369370371372373374375376377378379"
		"380381382383384385386387838389390391392393394395396397398399"
		"400401402403404405406407408409410411412413414415416417418419"
		"420421422423424425426427428429430431432433434435436437438439"
		"440441442443444445446447448449450451452453454455456457458459"
		"460461462463464465466467468469470471472473474475476477478479"
		"480481482483484485486487488489490491492493494495496497498499"
		"500501502503504505506507508509510511512513514515516517518519"
		"520521522523524525526527528529530531532533534535536537538539"
		"540541542543544545546547548549550551552553554555556557558559"
		"560561562563564565566567568569570571572573574575576577578579"
		"580581582583584585586587588589590591592593594595596597598599"
		"600601602603604605606607608609610611612613614615616617618619"
		"620621622623624625626627628629630631632633634635636637638639"
		"640641642643644645646647648649650651652653654655656657658659"
		"660661662663664665666667668669670671672673674675676677678679"
		"680681682683684685686687688689690691692693694695696697698699"
		"707701702703704705706707708709710711712713714715716717718719"
		"727721722723724725726727728729730731732733734735736737738739"
		"747741742743744745746747748749750751752753754755756757758759"
		"767761762763764765766767768769770771772773774775776777778779"
		"787781782783784785786787788789790791792793794795796797798799"
		"800801802803804805806807808809810811812813814815816817818819"
		"820821822823824825826827828829830831832833834835836837838839"
		"840841842843844845846847848849850851852853854855856857858859"
		"860861862863864865866867868869870871872873874875876877878879"
		"880881882883884885886887888889890891892893894895896897898899"
		"900901902903904905906907908909910911912913914915916917918919"
		"920921922923924925926927928929930931932933934935936937938939"
		"940941942943944945946947948949950951952953954955956957958959"
		"960961962963964965966967968969970971972973974975976977978979"
		"980981982983984985986987988989990991992993994995996997998999";

	static inline const char* HEX_LOOKUP =
		"000102030405060708090A0B0C0D0E0F"
		"101112131415161718191A1B1C1D1E1F"
		"202122232425262728292A2B2C2D2E2F"
		"303132333435363738393A3B3C3D3E3F"
		"404142434445464748494A4B4C4D4E4F"
		"505152535455565758595A5B5C5D5E5F"
		"606162636465666768696A6B6C6D6E6F"
		"707172737475767778797A7B7C7D7E7F"
		"808182838485868788898A8B8C8D8E8F"
		"909192939495969798999A9B9C9D9E9F"
		"A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
		"B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
		"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
		"D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
		"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
		"F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	static inline const char* ALPHANUM_LOOKUP =
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
};

template<>
struct StringLookup<char8_t>
{
	static inline const char8_t* TRUE_STR = u8"true";
	static inline const char8_t* FALSE_STR = u8"false";
	static inline const char8_t NULL_CHAR = u8'\0';
	static inline const char8_t NEGATIVE_CHAR = u8'-';
	static inline const char8_t POSITIVE_CHAR = u8'+';
	static inline const char8_t DECIMAL_CHAR = u8'.';
	static inline const char8_t ZERO_CHAR = u8'0';
	static inline const char8_t BACK_SLASH = u8'\\';
	static inline const char8_t FORWARD_SLASH = u8'/';
	static inline const char8_t SPACE = u8' ';
	static inline const char8_t HTAB = u8'\t';
	static inline const char8_t VTAB = u8'\v';
	static inline const char8_t RETURN = u8'\r';
	static inline const char8_t NEW_LINE = u8'\n';
	static inline const char8_t FEED = u8'\f';
	static inline const char8_t OPEN_BRACE = u8'{';
	static inline const char8_t CLOSE_BRACE = u8'}';
	static inline const char8_t FMT_HEX = u8'h';
	static inline const char8_t FMT_DEC = u8'.';

	static inline const char8_t* DECIMAL_LOOKUP =
		u8"000001002003004005006007008009010011012013014015016017018019"
		"020021022023024025026027028029030031032033034035036037038039"
		"040041042043044045046047048049050051052053054055056057058059"
		"060061062063064065066067068069070071072073074075076077078079"
		"080081082083084085086087088089090091092093094095096097098099"
		"100101102103104105106107108109110111112113114115116117118119"
		"120121122123124125126127128129130131132133134135136137138139"
		"140141142143144145146147148149150151152153154155156157158159"
		"160161162163164165166167168169170171172173174175176177178179"
		"180181182183184185186187188189190191192193194195196197198199"
		"200201202203204205206207208209210211212213214215216217218219"
		"220221222223224225226227228229230231232233234235236237238239"
		"240241242243244245246247248249250251252253254255256257258259"
		"260261262263264265266267268269270271272273274275276277278279"
		"280281282283284285286287288289290291292293294295296297298299"
		"300301302303304305306307038309310311312313314315316317318319"
		"320321322323324325326327238329330331332333334335336337338339"
		"340341342343344345346347438349350351352353354355356357358359"
		"360361362363364365366367638369370371372373374375376377378379"
		"380381382383384385386387838389390391392393394395396397398399"
		"400401402403404405406407408409410411412413414415416417418419"
		"420421422423424425426427428429430431432433434435436437438439"
		"440441442443444445446447448449450451452453454455456457458459"
		"460461462463464465466467468469470471472473474475476477478479"
		"480481482483484485486487488489490491492493494495496497498499"
		"500501502503504505506507508509510511512513514515516517518519"
		"520521522523524525526527528529530531532533534535536537538539"
		"540541542543544545546547548549550551552553554555556557558559"
		"560561562563564565566567568569570571572573574575576577578579"
		"580581582583584585586587588589590591592593594595596597598599"
		"600601602603604605606607608609610611612613614615616617618619"
		"620621622623624625626627628629630631632633634635636637638639"
		"640641642643644645646647648649650651652653654655656657658659"
		"660661662663664665666667668669670671672673674675676677678679"
		"680681682683684685686687688689690691692693694695696697698699"
		"707701702703704705706707708709710711712713714715716717718719"
		"727721722723724725726727728729730731732733734735736737738739"
		"747741742743744745746747748749750751752753754755756757758759"
		"767761762763764765766767768769770771772773774775776777778779"
		"787781782783784785786787788789790791792793794795796797798799"
		"800801802803804805806807808809810811812813814815816817818819"
		"820821822823824825826827828829830831832833834835836837838839"
		"840841842843844845846847848849850851852853854855856857858859"
		"860861862863864865866867868869870871872873874875876877878879"
		"880881882883884885886887888889890891892893894895896897898899"
		"900901902903904905906907908909910911912913914915916917918919"
		"920921922923924925926927928929930931932933934935936937938939"
		"940941942943944945946947948949950951952953954955956957958959"
		"960961962963964965966967968969970971972973974975976977978979"
		"980981982983984985986987988989990991992993994995996997998999";

	static inline const char8_t* HEX_LOOKUP =
		u8"000102030405060708090A0B0C0D0E0F"
		"101112131415161718191A1B1C1D1E1F"
		"202122232425262728292A2B2C2D2E2F"
		"303132333435363738393A3B3C3D3E3F"
		"404142434445464748494A4B4C4D4E4F"
		"505152535455565758595A5B5C5D5E5F"
		"606162636465666768696A6B6C6D6E6F"
		"707172737475767778797A7B7C7D7E7F"
		"808182838485868788898A8B8C8D8E8F"
		"909192939495969798999A9B9C9D9E9F"
		"A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
		"B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
		"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
		"D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
		"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
		"F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	static inline const char8_t* ALPHANUM_LOOKUP =
		u8"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
};

template<>
struct StringLookup<char16_t>
{
	static inline const char16_t* TRUE_STR = u"true";
	static inline const char16_t* FALSE_STR = u"false";
	static inline const char16_t NULL_CHAR = u'\0';
	static inline const char16_t NEGATIVE_CHAR = u'-';
	static inline const char16_t POSITIVE_CHAR = u'+';
	static inline const char16_t DECIMAL_CHAR = u'.';
	static inline const char16_t ZERO_CHAR = u'0';
	static inline const char16_t BACK_SLASH = u'\\';
	static inline const char16_t FORWARD_SLASH = u'/';
	static inline const char16_t SPACE = u' ';
	static inline const char16_t HTAB = u'\t';
	static inline const char16_t VTAB = u'\v';
	static inline const char16_t RETURN = u'\r';
	static inline const char16_t NEW_LINE = u'\n';
	static inline const char16_t FEED = u'\f';
	static inline const char16_t OPEN_BRACE = u'{';
	static inline const char16_t CLOSE_BRACE = u'}';
	static inline const char16_t FMT_HEX = u'h';
	static inline const char16_t FMT_DEC = u'.';

	static inline const char16_t* DECIMAL_LOOKUP =
		u"000001002003004005006007008009010011012013014015016017018019"
		"020021022023024025026027028029030031032033034035036037038039"
		"040041042043044045046047048049050051052053054055056057058059"
		"060061062063064065066067068069070071072073074075076077078079"
		"080081082083084085086087088089090091092093094095096097098099"
		"100101102103104105106107108109110111112113114115116117118119"
		"120121122123124125126127128129130131132133134135136137138139"
		"140141142143144145146147148149150151152153154155156157158159"
		"160161162163164165166167168169170171172173174175176177178179"
		"180181182183184185186187188189190191192193194195196197198199"
		"200201202203204205206207208209210211212213214215216217218219"
		"220221222223224225226227228229230231232233234235236237238239"
		"240241242243244245246247248249250251252253254255256257258259"
		"260261262263264265266267268269270271272273274275276277278279"
		"280281282283284285286287288289290291292293294295296297298299"
		"300301302303304305306307038309310311312313314315316317318319"
		"320321322323324325326327238329330331332333334335336337338339"
		"340341342343344345346347438349350351352353354355356357358359"
		"360361362363364365366367638369370371372373374375376377378379"
		"380381382383384385386387838389390391392393394395396397398399"
		"400401402403404405406407408409410411412413414415416417418419"
		"420421422423424425426427428429430431432433434435436437438439"
		"440441442443444445446447448449450451452453454455456457458459"
		"460461462463464465466467468469470471472473474475476477478479"
		"480481482483484485486487488489490491492493494495496497498499"
		"500501502503504505506507508509510511512513514515516517518519"
		"520521522523524525526527528529530531532533534535536537538539"
		"540541542543544545546547548549550551552553554555556557558559"
		"560561562563564565566567568569570571572573574575576577578579"
		"580581582583584585586587588589590591592593594595596597598599"
		"600601602603604605606607608609610611612613614615616617618619"
		"620621622623624625626627628629630631632633634635636637638639"
		"640641642643644645646647648649650651652653654655656657658659"
		"660661662663664665666667668669670671672673674675676677678679"
		"680681682683684685686687688689690691692693694695696697698699"
		"707701702703704705706707708709710711712713714715716717718719"
		"727721722723724725726727728729730731732733734735736737738739"
		"747741742743744745746747748749750751752753754755756757758759"
		"767761762763764765766767768769770771772773774775776777778779"
		"787781782783784785786787788789790791792793794795796797798799"
		"800801802803804805806807808809810811812813814815816817818819"
		"820821822823824825826827828829830831832833834835836837838839"
		"840841842843844845846847848849850851852853854855856857858859"
		"860861862863864865866867868869870871872873874875876877878879"
		"880881882883884885886887888889890891892893894895896897898899"
		"900901902903904905906907908909910911912913914915916917918919"
		"920921922923924925926927928929930931932933934935936937938939"
		"940941942943944945946947948949950951952953954955956957958959"
		"960961962963964965966967968969970971972973974975976977978979"
		"980981982983984985986987988989990991992993994995996997998999";

	static inline const char16_t* HEX_LOOKUP =
		u"000102030405060708090A0B0C0D0E0F"
		"101112131415161718191A1B1C1D1E1F"
		"202122232425262728292A2B2C2D2E2F"
		"303132333435363738393A3B3C3D3E3F"
		"404142434445464748494A4B4C4D4E4F"
		"505152535455565758595A5B5C5D5E5F"
		"606162636465666768696A6B6C6D6E6F"
		"707172737475767778797A7B7C7D7E7F"
		"808182838485868788898A8B8C8D8E8F"
		"909192939495969798999A9B9C9D9E9F"
		"A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
		"B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
		"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
		"D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
		"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
		"F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	static inline const char16_t* ALPHANUM_LOOKUP =
		u"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
};

template<>
struct StringLookup<char32_t>
{
	static inline const char32_t* TRUE_STR = U"true";
	static inline const char32_t* FALSE_STR = U"false";
	static inline const char32_t NULL_CHAR = U'\0';
	static inline const char32_t NEGATIVE_CHAR = U'-';
	static inline const char32_t POSITIVE_CHAR = U'+';
	static inline const char32_t DECIMAL_CHAR = U'.';
	static inline const char32_t ZERO_CHAR = U'0';
	static inline const char32_t BACK_SLASH = U'\\';
	static inline const char32_t FORWARD_SLASH = U'/';
	static inline const char32_t SPACE = U' ';
	static inline const char32_t HTAB = U'\t';
	static inline const char32_t VTAB = U'\v';
	static inline const char32_t RETURN = U'\r';
	static inline const char32_t NEW_LINE = U'\n';
	static inline const char32_t FEED = U'\f';
	static inline const char32_t OPEN_BRACE = U'{';
	static inline const char32_t CLOSE_BRACE = U'}';
	static inline const char32_t FMT_HEX = U'h';
	static inline const char32_t FMT_DEC = U'.';

	static inline const char32_t* DECIMAL_LOOKUP =
		U"000001002003004005006007008009010011012013014015016017018019"
		"020021022023024025026027028029030031032033034035036037038039"
		"040041042043044045046047048049050051052053054055056057058059"
		"060061062063064065066067068069070071072073074075076077078079"
		"080081082083084085086087088089090091092093094095096097098099"
		"100101102103104105106107108109110111112113114115116117118119"
		"120121122123124125126127128129130131132133134135136137138139"
		"140141142143144145146147148149150151152153154155156157158159"
		"160161162163164165166167168169170171172173174175176177178179"
		"180181182183184185186187188189190191192193194195196197198199"
		"200201202203204205206207208209210211212213214215216217218219"
		"220221222223224225226227228229230231232233234235236237238239"
		"240241242243244245246247248249250251252253254255256257258259"
		"260261262263264265266267268269270271272273274275276277278279"
		"280281282283284285286287288289290291292293294295296297298299"
		"300301302303304305306307038309310311312313314315316317318319"
		"320321322323324325326327238329330331332333334335336337338339"
		"340341342343344345346347438349350351352353354355356357358359"
		"360361362363364365366367638369370371372373374375376377378379"
		"380381382383384385386387838389390391392393394395396397398399"
		"400401402403404405406407408409410411412413414415416417418419"
		"420421422423424425426427428429430431432433434435436437438439"
		"440441442443444445446447448449450451452453454455456457458459"
		"460461462463464465466467468469470471472473474475476477478479"
		"480481482483484485486487488489490491492493494495496497498499"
		"500501502503504505506507508509510511512513514515516517518519"
		"520521522523524525526527528529530531532533534535536537538539"
		"540541542543544545546547548549550551552553554555556557558559"
		"560561562563564565566567568569570571572573574575576577578579"
		"580581582583584585586587588589590591592593594595596597598599"
		"600601602603604605606607608609610611612613614615616617618619"
		"620621622623624625626627628629630631632633634635636637638639"
		"640641642643644645646647648649650651652653654655656657658659"
		"660661662663664665666667668669670671672673674675676677678679"
		"680681682683684685686687688689690691692693694695696697698699"
		"707701702703704705706707708709710711712713714715716717718719"
		"727721722723724725726727728729730731732733734735736737738739"
		"747741742743744745746747748749750751752753754755756757758759"
		"767761762763764765766767768769770771772773774775776777778779"
		"787781782783784785786787788789790791792793794795796797798799"
		"800801802803804805806807808809810811812813814815816817818819"
		"820821822823824825826827828829830831832833834835836837838839"
		"840841842843844845846847848849850851852853854855856857858859"
		"860861862863864865866867868869870871872873874875876877878879"
		"880881882883884885886887888889890891892893894895896897898899"
		"900901902903904905906907908909910911912913914915916917918919"
		"920921922923924925926927928929930931932933934935936937938939"
		"940941942943944945946947948949950951952953954955956957958959"
		"960961962963964965966967968969970971972973974975976977978979"
		"980981982983984985986987988989990991992993994995996997998999";

	static inline const char32_t* HEX_LOOKUP =
		U"000102030405060708090A0B0C0D0E0F"
		"101112131415161718191A1B1C1D1E1F"
		"202122232425262728292A2B2C2D2E2F"
		"303132333435363738393A3B3C3D3E3F"
		"404142434445464748494A4B4C4D4E4F"
		"505152535455565758595A5B5C5D5E5F"
		"606162636465666768696A6B6C6D6E6F"
		"707172737475767778797A7B7C7D7E7F"
		"808182838485868788898A8B8C8D8E8F"
		"909192939495969798999A9B9C9D9E9F"
		"A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
		"B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
		"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
		"D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
		"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
		"F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	static inline const char32_t* ALPHANUM_LOOKUP =
		U"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
};

template<>
struct StringLookup<wchar_t>
{
	static inline const wchar_t* TRUE_STR = L"true";
	static inline const wchar_t* FALSE_STR = L"false";
	static inline const wchar_t NULL_CHAR = L'\0';
	static inline const wchar_t NEGATIVE_CHAR = L'-';
	static inline const wchar_t POSITIVE_CHAR = L'+';
	static inline const wchar_t DECIMAL_CHAR = L'.';
	static inline const wchar_t ZERO_CHAR = L'0';
	static inline const wchar_t BACK_SLASH = L'\\';
	static inline const wchar_t FORWARD_SLASH = L'/';
	static inline const wchar_t SPACE = L' ';
	static inline const wchar_t HTAB = L'\t';
	static inline const wchar_t VTAB = L'\v';
	static inline const wchar_t RETURN = L'\r';
	static inline const wchar_t NEW_LINE = L'\n';
	static inline const wchar_t FEED = L'\f';
	static inline const wchar_t OPEN_BRACE = L'{';
	static inline const wchar_t CLOSE_BRACE = L'}';
	static inline const wchar_t FMT_HEX = L'h';
	static inline const wchar_t FMT_DEC = L'.';

	static inline const wchar_t* DECIMAL_LOOKUP =
		L"000001002003004005006007008009010011012013014015016017018019"
		"020021022023024025026027028029030031032033034035036037038039"
		"040041042043044045046047048049050051052053054055056057058059"
		"060061062063064065066067068069070071072073074075076077078079"
		"080081082083084085086087088089090091092093094095096097098099"
		"100101102103104105106107108109110111112113114115116117118119"
		"120121122123124125126127128129130131132133134135136137138139"
		"140141142143144145146147148149150151152153154155156157158159"
		"160161162163164165166167168169170171172173174175176177178179"
		"180181182183184185186187188189190191192193194195196197198199"
		"200201202203204205206207208209210211212213214215216217218219"
		"220221222223224225226227228229230231232233234235236237238239"
		"240241242243244245246247248249250251252253254255256257258259"
		"260261262263264265266267268269270271272273274275276277278279"
		"280281282283284285286287288289290291292293294295296297298299"
		"300301302303304305306307038309310311312313314315316317318319"
		"320321322323324325326327238329330331332333334335336337338339"
		"340341342343344345346347438349350351352353354355356357358359"
		"360361362363364365366367638369370371372373374375376377378379"
		"380381382383384385386387838389390391392393394395396397398399"
		"400401402403404405406407408409410411412413414415416417418419"
		"420421422423424425426427428429430431432433434435436437438439"
		"440441442443444445446447448449450451452453454455456457458459"
		"460461462463464465466467468469470471472473474475476477478479"
		"480481482483484485486487488489490491492493494495496497498499"
		"500501502503504505506507508509510511512513514515516517518519"
		"520521522523524525526527528529530531532533534535536537538539"
		"540541542543544545546547548549550551552553554555556557558559"
		"560561562563564565566567568569570571572573574575576577578579"
		"580581582583584585586587588589590591592593594595596597598599"
		"600601602603604605606607608609610611612613614615616617618619"
		"620621622623624625626627628629630631632633634635636637638639"
		"640641642643644645646647648649650651652653654655656657658659"
		"660661662663664665666667668669670671672673674675676677678679"
		"680681682683684685686687688689690691692693694695696697698699"
		"707701702703704705706707708709710711712713714715716717718719"
		"727721722723724725726727728729730731732733734735736737738739"
		"747741742743744745746747748749750751752753754755756757758759"
		"767761762763764765766767768769770771772773774775776777778779"
		"787781782783784785786787788789790791792793794795796797798799"
		"800801802803804805806807808809810811812813814815816817818819"
		"820821822823824825826827828829830831832833834835836837838839"
		"840841842843844845846847848849850851852853854855856857858859"
		"860861862863864865866867868869870871872873874875876877878879"
		"880881882883884885886887888889890891892893894895896897898899"
		"900901902903904905906907908909910911912913914915916917918919"
		"920921922923924925926927928929930931932933934935936937938939"
		"940941942943944945946947948949950951952953954955956957958959"
		"960961962963964965966967968969970971972973974975976977978979"
		"980981982983984985986987988989990991992993994995996997998999";

	static inline const wchar_t* HEX_LOOKUP =
		L"000102030405060708090A0B0C0D0E0F"
		"101112131415161718191A1B1C1D1E1F"
		"202122232425262728292A2B2C2D2E2F"
		"303132333435363738393A3B3C3D3E3F"
		"404142434445464748494A4B4C4D4E4F"
		"505152535455565758595A5B5C5D5E5F"
		"606162636465666768696A6B6C6D6E6F"
		"707172737475767778797A7B7C7D7E7F"
		"808182838485868788898A8B8C8D8E8F"
		"909192939495969798999A9B9C9D9E9F"
		"A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
		"B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
		"C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
		"D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
		"E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
		"F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	static inline const wchar_t* ALPHANUM_LOOKUP =
		L"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
};