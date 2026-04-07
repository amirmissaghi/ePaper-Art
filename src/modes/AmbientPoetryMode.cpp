// AmbientPoetryMode.cpp — Poetry overlay with curated quotes
#include "modes/AmbientPoetryMode.h"
#include "config.h"
#include <Fonts/FreeSerif9pt7b.h>
#include <cmath>

// ── Curated quote collection with theme tags ─────────────
// ~250 quotes: poetry fragments, haiku, proverbs, philosophy
// Each line is pre-wrapped to fit the 260px-wide display area.

static const Quote QUOTES[] = {

// ═══════════════════════════════════════════════════════════
//  NATURE — Botanist (plants, organic growth, gardens)
// ═══════════════════════════════════════════════════════════

{"The world is full\nof magic things,\npatiently waiting\nfor our senses\nto grow sharper.\n— W.B. Yeats", QuoteTheme::NATURE},
{"Even the tiniest\nflower can have\nthe toughest roots.\n— Shannon Mullen", QuoteTheme::NATURE},
{"In seed time\nlearn, in harvest\nteach, in winter\nenjoy.\n— William Blake", QuoteTheme::NATURE},
{"The earth has\nmusic for those\nwho listen.\n— Shakespeare", QuoteTheme::NATURE},
{"Where flowers bloom,\nso does hope.\n— Lady Bird Johnson", QuoteTheme::NATURE},
{"Look deep\ninto nature,\nand then you\nwill understand\neverything better.\n— Albert Einstein", QuoteTheme::NATURE},
{"In every walk\nwith nature one\nreceives far more\nthan he seeks.\n— John Muir", QuoteTheme::NATURE},
{"The clearest way\ninto the Universe\nis through\na forest wilderness.\n— John Muir", QuoteTheme::NATURE},
{"He who plants\na tree plants\na hope.\n— Lucy Larcom", QuoteTheme::NATURE},
{"Adopt the pace\nof nature: her\nsecret is patience.\n— Ralph Waldo Emerson", QuoteTheme::NATURE},
{"Nature does not\nhurry, yet\neverything is\naccomplished.\n— Lao Tzu", QuoteTheme::NATURE},
{"The poetry of\nearth is\nnever dead.\n— John Keats", QuoteTheme::NATURE},
{"I go to nature\nto be soothed\nand healed.\n— John Burroughs", QuoteTheme::NATURE},
{"To forget how\nto dig the earth\nis to forget\nourselves.\n— Mahatma Gandhi", QuoteTheme::NATURE},
{"One touch of\nnature makes the\nwhole world kin.\n— Shakespeare", QuoteTheme::NATURE},
{"The creation of\na thousand forests\nis in one acorn.\n— Ralph Waldo Emerson", QuoteTheme::NATURE},
{"Trees are poems\nthat the earth\nwrites upon\nthe sky.\n— Kahlil Gibran", QuoteTheme::NATURE},
{"The flower that\nblooms in adversity\nis the rarest and\nmost beautiful\nof all.\n— Mulan", QuoteTheme::NATURE},
{"Study nature,\nlove nature,\nstay close\nto nature.\nIt will never\nfail you.\n— Frank Lloyd Wright", QuoteTheme::NATURE},
{"Just living\nis not enough.\nOne must have\nsunshine, freedom,\nand a little flower.\n— Hans Christian\n  Andersen", QuoteTheme::NATURE},
{"A morning glory\nat my window\nsatisfies me more\nthan the metaphysics\nof books.\n— Walt Whitman", QuoteTheme::NATURE},
{"The earth laughs\nin flowers.\n— Ralph Waldo Emerson", QuoteTheme::NATURE},
{"To see a world\nin a grain of sand\nand heaven\nin a wildflower.\n— William Blake", QuoteTheme::NATURE},
{"Blossom by blossom\nthe spring\nbegins.\n— Algernon Charles\n  Swinburne", QuoteTheme::NATURE},
{"Green fingers\nare the extension\nof a verdant heart.\n— Russell Page", QuoteTheme::NATURE},
{"If you have\na garden and\na library, you\nhave everything\nyou need.\n— Cicero", QuoteTheme::NATURE},
{"Deep in their\nroots, all flowers\nkeep the light.\n— Theodore Roethke", QuoteTheme::NATURE},
{"The best time\nto plant a tree\nwas twenty\nyears ago.\nThe second best\ntime is now.\n— Chinese Proverb", QuoteTheme::NATURE},
{"In the spring,\nat the end\nof the day, you\nshould smell\nlike dirt.\n— Margaret Atwood", QuoteTheme::NATURE},
{"What is a weed?\nA plant whose\nvirtues have not\nyet been\ndiscovered.\n— Ralph Waldo Emerson", QuoteTheme::NATURE},
{"A single rose\ncan be my garden;\na single friend,\nmy world.\n— Leo Buscaglia", QuoteTheme::NATURE},
{"Moss grows\nwhere nothing\nelse will.\n— Japanese Saying", QuoteTheme::NATURE},
{"No winter lasts\nforever; no spring\nskips its turn.\n— Hal Borland", QuoteTheme::NATURE},
{"Old wood\nbest to burn,\nold wine to drink,\nold friends\nto trust.\n— Francis Bacon", QuoteTheme::NATURE},
{"All the flowers\nof all the\ntomorrows are in\nthe seeds of today.\n— Indian Proverb", QuoteTheme::NATURE},

// ═══════════════════════════════════════════════════════════
//  SKY — Mood Window (atmospheric, weather, light)
// ═══════════════════════════════════════════════════════════

{"Stars, hide\nyour fires; let not\nlight see my dark\nand deep desires.\n— Shakespeare", QuoteTheme::SKY},
{"Keep your face\nalways toward\nthe sunshine,\nand shadows will\nfall behind you.\n— Walt Whitman", QuoteTheme::SKY},
{"The moon does not\nfight. It attacks\nno one. And yet\nit does not need to.\n— Deng Ming-Dao", QuoteTheme::SKY},
{"Light tomorrow\nwith today.\n— Elizabeth Barrett\n  Browning", QuoteTheme::SKY},
{"Every sunset brings\nthe promise of\na new dawn.\n— Ralph Waldo Emerson", QuoteTheme::SKY},
{"I have loved\nthe stars too\nfondly to be\nfearful of\nthe night.\n— Sarah Williams", QuoteTheme::SKY},
{"After rain\ncomes the sun.\nAfter darkness\ncomes the\nglorious light.\n— Rumi", QuoteTheme::SKY},
{"Clouds come\nfloating into\nmy life, to add\ncolor to my\nsunset sky.\n— Tagore", QuoteTheme::SKY},
{"When it is dark\nenough, you can\nsee the stars.\n— Ralph Waldo Emerson", QuoteTheme::SKY},
{"Moonlight drowns\nout all but\nthe brightest\nstars.\n— J.R.R. Tolkien", QuoteTheme::SKY},
{"At night I dream\nthat you and I\nare two plants\nthat grew together.\n— Pablo Neruda", QuoteTheme::SKY},
{"We are all\nin the gutter,\nbut some of us\nare looking at\nthe stars.\n— Oscar Wilde", QuoteTheme::SKY},
{"There are always\nflowers for those\nwho want to\nsee them.\n— Henri Matisse", QuoteTheme::SKY},
{"The sun himself\nis weak when he\nfirst rises, and\ngathers strength\nas the day\ngets on.\n— Charles Dickens", QuoteTheme::SKY},
{"I am a part of\nall that I\nhave met.\n— Alfred Tennyson", QuoteTheme::SKY},
{"Give light\nand people\nwill find the way.\n— Ella Baker", QuoteTheme::SKY},
{"Darkness cannot\ndrive out\ndarkness;\nonly light\ncan do that.\n— Martin Luther\n  King Jr.", QuoteTheme::SKY},
{"Even the darkest\nnight will end\nand the sun\nwill rise.\n— Victor Hugo", QuoteTheme::SKY},
{"The sky is the\ndaily bread\nof the eyes.\n— Ralph Waldo Emerson", QuoteTheme::SKY},
{"Dwell on the\nbeauty of life.\nWatch the stars,\nand see yourself\nrunning with them.\n— Marcus Aurelius", QuoteTheme::SKY},
{"There is nothing\nin a caterpillar\nthat tells you\nit's going to be\na butterfly.\n— Buckminster\n  Fuller", QuoteTheme::SKY},
{"May you always\nhave walls for\nthe winds, a roof\nfor the rain,\nand sunshine\nwarm upon\nyour face.\n— Irish Blessing", QuoteTheme::SKY},
{"The wound is\nthe place where\nthe light\nenters you.\n— Rumi", QuoteTheme::SKY},
{"What sunshine is\nto flowers,\nsmiles are\nto humanity.\n— Joseph Addison", QuoteTheme::SKY},
{"Who has seen\nthe wind?\nNeither I nor you.\nBut when the\ntrees bow down,\nthe wind is\npassing through.\n— Christina Rossetti", QuoteTheme::SKY},
{"Be like the sun\nand meadow,\nwhich are not\nin the least\nconcerned about\nthe coming winter.\n— George Bernard\n  Shaw", QuoteTheme::SKY},
{"A sunrise is\nGod's way of\nsaying: Let's\nstart again.\n— Todd Stocker", QuoteTheme::SKY},
{"Aim for the moon.\nIf you miss,\nyou may hit\na star.\n— W. Clement Stone", QuoteTheme::SKY},
{"Turn your face\nto the sun and\nthe shadows fall\nbehind you.\n— Maori Proverb", QuoteTheme::SKY},
{"The sky takes on\nshades of orange\nduring sunrise\nand sunset.\n— Ram Charan", QuoteTheme::SKY},

// ═══════════════════════════════════════════════════════════
//  WATER — DailyCanvas (flow, patterns, haiku)
// ═══════════════════════════════════════════════════════════

{"Nothing is softer\nor more flexible\nthan water, yet\nnothing can\nresist it.\n— Lao Tzu", QuoteTheme::WATER},
{"A world of dew,\nand within every\ndewdrop\na world of struggle.\n— Issa", QuoteTheme::WATER},
{"The old pond—\na frog jumps in,\nsound of water.\n— Basho", QuoteTheme::WATER},
{"Over the wintry\nforest, winds howl\nin rage with no\nleaves to blow.\n— Natsume Soseki", QuoteTheme::WATER},
{"Autumn moonlight—\na worm digs silently\ninto the chestnut.\n— Basho", QuoteTheme::WATER},
{"Empty your mind.\nBe formless,\nshapeless,\nlike water.\n— Bruce Lee", QuoteTheme::WATER},
{"Water is the\ndriving force\nof all nature.\n— Leonardo da Vinci", QuoteTheme::WATER},
{"The cure for\nanything is\nsalt water:\nsweat, tears,\nor the sea.\n— Isak Dinesen", QuoteTheme::WATER},
{"Rivers know this:\nthere is no hurry.\nWe shall get\nthere someday.\n— A.A. Milne", QuoteTheme::WATER},
{"The sea, once it\ncasts its spell,\nholds one in\nits net of wonder\nforever.\n— Jacques Cousteau", QuoteTheme::WATER},
{"In one drop\nof water are\nfound all the\nsecrets of\nall the oceans.\n— Kahlil Gibran", QuoteTheme::WATER},
{"Still waters\nrun deep.\n— Latin Proverb", QuoteTheme::WATER},
{"Be water,\nmy friend.\n— Bruce Lee", QuoteTheme::WATER},
{"The fall of\nrain is the\nvoice of memory.\n— Yoko Ogawa", QuoteTheme::WATER},
{"In the midst\nof movement\nand chaos, keep\nstillness inside\nof you.\n— Deepak Chopra", QuoteTheme::WATER},
{"A lake is the\nlandscape's most\nbeautiful\nand expressive\nfeature.\n— Henry David\n  Thoreau", QuoteTheme::WATER},
{"You can't cross\nthe sea merely\nby standing and\nstaring at\nthe water.\n— Rabindranath Tagore", QuoteTheme::WATER},
{"I must be\na mermaid.\nI have no fear\nof depths and\na great fear of\nshallow living.\n— Anais Nin", QuoteTheme::WATER},
{"The voice\nof the sea speaks\nto the soul.\n— Kate Chopin", QuoteTheme::WATER},
{"Temple bell\nstops but I still\nhear the sound\ncoming out\nof the flowers.\n— Basho", QuoteTheme::WATER},
{"The snow falling—\njust enough\nto make the\npostman's tracks.\n— Buson", QuoteTheme::WATER},
{"A bee\nstaggers out\nof the peony.\n— Basho", QuoteTheme::WATER},
{"Come, let us go;\nlet us find out\nwhat the rain\nis doing.\n— Lucy Maud\n  Montgomery", QuoteTheme::WATER},
{"Spring rain—\na child teaches\nthe cat to dance.\n— Issa", QuoteTheme::WATER},
{"Lightning flash—\nwhat I thought\nwere faces\nare plumes of\npampas grass.\n— Basho", QuoteTheme::WATER},
{"How still\nthe water of\nthis old stone pool,\nuntouched.\n— Buson", QuoteTheme::WATER},
{"Even in Kyoto—\nhearing the\ncuckoo's cry—\nI long for Kyoto.\n— Basho", QuoteTheme::WATER},
{"Don't cry because\nit's over;\nsmile because\nit happened.\n— Dr. Seuss", QuoteTheme::WATER},
{"My barn having\nburned down,\nI can now see\nthe moon.\n— Mizuta Masahide", QuoteTheme::WATER},
{"The bamboo that\nbends is stronger\nthan the oak\nthat resists.\n— Japanese Proverb", QuoteTheme::WATER},
{"Sitting quietly,\ndoing nothing,\nspring comes,\nand the grass\ngrows by itself.\n— Zenrin", QuoteTheme::WATER},
{"Fall seven times,\nstand up eight.\n— Japanese Proverb", QuoteTheme::WATER},
{"When you drink\nwater, remember\nthe spring.\n— Chinese Proverb", QuoteTheme::WATER},
{"Dripping water\nhollows out stone,\nnot through\nforce but through\npersistence.\n— Ovid", QuoteTheme::WATER},
{"No man ever\nsteps in the\nsame river twice.\n— Heraclitus", QuoteTheme::WATER},
{"The river\nthat flows in you\nalso flows\nin me.\n— Kabir", QuoteTheme::WATER},
{"Rain does not\nfall on one\nroof alone.\n— Cameroonian\n  Proverb", QuoteTheme::WATER},
{"Between the wish\nand the thing\nthe world lies\nwaiting.\n— Cormac McCarthy", QuoteTheme::WATER},
{"Water does not\nresist.\nWater flows.\n— Margaret Atwood", QuoteTheme::WATER},

// ═══════════════════════════════════════════════════════════
//  ABSTRACT — DailyCanvas (geometry, thought, philosophy)
// ═══════════════════════════════════════════════════════════

{"Simplicity is\nthe ultimate\nsophistication.\n— Leonardo da Vinci", QuoteTheme::ABSTRACT},
{"In the middle\nof difficulty\nlies opportunity.\n— Albert Einstein", QuoteTheme::ABSTRACT},
{"The quieter\nyou become,\nthe more you\ncan hear.\n— Ram Dass", QuoteTheme::ABSTRACT},
{"Every moment\nis a fresh\nbeginning.\n— T.S. Eliot", QuoteTheme::ABSTRACT},
{"Do small things\nwith great love.\n— Mother Teresa", QuoteTheme::ABSTRACT},
{"Be yourself;\neveryone else\nis already taken.\n— Oscar Wilde", QuoteTheme::ABSTRACT},
{"Everything has\nbeauty, but not\neveryone sees it.\n— Confucius", QuoteTheme::ABSTRACT},
{"Less is more.\n— Ludwig Mies\n  van der Rohe", QuoteTheme::ABSTRACT},
{"Imagination is\nmore important\nthan knowledge.\n— Albert Einstein", QuoteTheme::ABSTRACT},
{"To be or not\nto be, that is\nthe question.\n— Shakespeare", QuoteTheme::ABSTRACT},
{"The unexamined\nlife is not\nworth living.\n— Socrates", QuoteTheme::ABSTRACT},
{"What we know\nis a drop.\nWhat we don't know\nis an ocean.\n— Isaac Newton", QuoteTheme::ABSTRACT},
{"Art is not\nwhat you see,\nbut what you\nmake others see.\n— Edgar Degas", QuoteTheme::ABSTRACT},
{"Music is the\nsilence between\nthe notes.\n— Claude Debussy", QuoteTheme::ABSTRACT},
{"Perfection is\nachieved not when\nthere is nothing\nmore to add,\nbut when nothing\nis left to\ntake away.\n— Antoine de\n  Saint-Exupery", QuoteTheme::ABSTRACT},
{"The only true\nwisdom is in\nknowing you\nknow nothing.\n— Socrates", QuoteTheme::ABSTRACT},
{"Turn your wounds\ninto wisdom.\n— Oprah Winfrey", QuoteTheme::ABSTRACT},
{"I think,\ntherefore\nI am.\n— Rene Descartes", QuoteTheme::ABSTRACT},
{"We are what\nwe repeatedly do.\nExcellence then\nis not an act,\nbut a habit.\n— Aristotle", QuoteTheme::ABSTRACT},
{"The mind is\neverything.\nWhat you think\nyou become.\n— Buddha", QuoteTheme::ABSTRACT},
{"He who has\na why to live\ncan bear almost\nany how.\n— Friedrich\n  Nietzsche", QuoteTheme::ABSTRACT},
{"To know\nand not to do\nis not yet\nto know.\n— Zen Proverb", QuoteTheme::ABSTRACT},
{"The obstacle\nis the way.\n— Marcus Aurelius", QuoteTheme::ABSTRACT},
{"We suffer more\noften in\nimagination\nthan in reality.\n— Seneca", QuoteTheme::ABSTRACT},
{"You must be\nthe change you\nwish to see\nin the world.\n— Mahatma Gandhi", QuoteTheme::ABSTRACT},
{"There is no\npath to happiness.\nHappiness is\nthe path.\n— Buddha", QuoteTheme::ABSTRACT},
{"Knowing yourself\nis the beginning\nof all wisdom.\n— Aristotle", QuoteTheme::ABSTRACT},
{"What you seek\nis seeking you.\n— Rumi", QuoteTheme::ABSTRACT},
{"The only limit\nto our realization\nof tomorrow will\nbe our doubts\nof today.\n— Franklin D.\n  Roosevelt", QuoteTheme::ABSTRACT},
{"Ring the bells\nthat still\ncan ring. Forget\nyour perfect\noffering. There is\na crack in\neverything.\n— Leonard Cohen", QuoteTheme::ABSTRACT},
{"How wonderful it\nis that nobody\nneed wait a\nsingle moment\nbefore starting\nto improve\nthe world.\n— Anne Frank", QuoteTheme::ABSTRACT},
{"Everything should\nbe made as\nsimple as\npossible, but\nnot simpler.\n— Albert Einstein", QuoteTheme::ABSTRACT},
{"Life is really\nsimple, but\nmen insist on\nmaking it\ncomplicated.\n— Confucius", QuoteTheme::ABSTRACT},
{"The measure of\nintelligence is\nthe ability\nto change.\n— Albert Einstein", QuoteTheme::ABSTRACT},
{"Have patience.\nAll things are\ndifficult before\nthey become easy.\n— Saadi", QuoteTheme::ABSTRACT},
{"Act as if what\nyou do makes a\ndifference.\nIt does.\n— William James", QuoteTheme::ABSTRACT},
{"A man is but\nthe product\nof his thoughts.\nWhat he thinks,\nhe becomes.\n— Mahatma Gandhi", QuoteTheme::ABSTRACT},
{"No man is wise\nenough by himself.\n— Plautus", QuoteTheme::ABSTRACT},
{"Between stimulus\nand response\nthere is a space.\nIn that space\nis our freedom.\n— Viktor Frankl", QuoteTheme::ABSTRACT},
{"Happiness depends\nupon ourselves.\n— Aristotle", QuoteTheme::ABSTRACT},
{"It is during\nour darkest\nmoments that we\nmust focus to\nsee the light.\n— Aristotle", QuoteTheme::ABSTRACT},
{"The only way\nout is through.\n— Robert Frost", QuoteTheme::ABSTRACT},
{"With the new\nday comes new\nstrength and\nnew thoughts.\n— Eleanor Roosevelt", QuoteTheme::ABSTRACT},
{"The best way\nout is always\nthrough.\n— Robert Frost", QuoteTheme::ABSTRACT},
{"To live is the\nrarest thing\nin the world.\nMost people\njust exist.\n— Oscar Wilde", QuoteTheme::ABSTRACT},
{"Our life is\nshaped by our\nmind; we become\nwhat we think.\n— Buddha", QuoteTheme::ABSTRACT},
{"If you judge\npeople, you have\nno time to\nlove them.\n— Mother Teresa", QuoteTheme::ABSTRACT},
{"We don't see\nthings as they are,\nwe see them\nas we are.\n— Anais Nin", QuoteTheme::ABSTRACT},
{"The purpose\nof art is\nwashing the\ndust of daily\nlife off\nour souls.\n— Pablo Picasso", QuoteTheme::ABSTRACT},
{"A room without\nbooks is like\na body without\na soul.\n— Marcus Tullius\n  Cicero", QuoteTheme::ABSTRACT},
{"Silence is a\ntrue friend who\nnever betrays.\n— Confucius", QuoteTheme::ABSTRACT},

// ═══════════════════════════════════════════════════════════
//  SCENE — Story Canvas (landscapes, journeys, wandering)
// ═══════════════════════════════════════════════════════════

{"Not all those\nwho wander\nare lost.\n— J.R.R. Tolkien", QuoteTheme::SCENE},
{"Life is either\na daring adventure\nor nothing at all.\n— Helen Keller", QuoteTheme::SCENE},
{"First autumn\nmorning: the mirror\nI stare into\nshows my\nfather's face.\n— Murakami Kijo", QuoteTheme::SCENE},
{"Two roads diverged\nin a wood, and I—\nI took the one\nless traveled by.\n— Robert Frost", QuoteTheme::SCENE},
{"The journey of\na thousand miles\nbegins with\na single step.\n— Lao Tzu", QuoteTheme::SCENE},
{"To travel is\nto live.\n— Hans Christian\n  Andersen", QuoteTheme::SCENE},
{"The world is\na book, and those\nwho do not travel\nread only\none page.\n— Saint Augustine", QuoteTheme::SCENE},
{"Wherever you go,\ngo with all\nyour heart.\n— Confucius", QuoteTheme::SCENE},
{"I wandered lonely\nas a cloud that\nfloats on high\no'er vales\nand hills.\n— William\n  Wordsworth", QuoteTheme::SCENE},
{"All that is gold\ndoes not glitter.\n— J.R.R. Tolkien", QuoteTheme::SCENE},
{"We shall not\ncease from\nexploration.\n— T.S. Eliot", QuoteTheme::SCENE},
{"A journey is\nbest measured\nin friends,\nrather than miles.\n— Tim Cahill", QuoteTheme::SCENE},
{"If you don't know\nwhere you are\ngoing, any road\nwill get\nyou there.\n— Lewis Carroll", QuoteTheme::SCENE},
{"The real voyage\nof discovery\nconsists not in\nseeking new\nlandscapes, but\nin having\nnew eyes.\n— Marcel Proust", QuoteTheme::SCENE},
{"Travel makes one\nmodest. You see\nwhat a tiny place\nyou occupy\nin the world.\n— Gustave Flaubert", QuoteTheme::SCENE},
{"We travel not\nto escape life,\nbut for life\nnot to escape us.\n— Anonymous", QuoteTheme::SCENE},
{"Perhaps travel\ncannot prevent\nbigotry, but it\nestablishes that\nall peoples cry,\nlaugh, eat.\n— Mark Twain", QuoteTheme::SCENE},
{"One's destination\nis never a place,\nbut a new way\nof seeing things.\n— Henry Miller", QuoteTheme::SCENE},
{"He who returns\nfrom a journey\nis not the same\nas he who left.\n— Chinese Proverb", QuoteTheme::SCENE},
{"Adventure is\nworthwhile\nin itself.\n— Amelia Earhart", QuoteTheme::SCENE},
{"Do not follow\nwhere the path\nmay lead. Go\ninstead where\nthere is no path\nand leave\na trail.\n— Ralph Waldo Emerson", QuoteTheme::SCENE},
{"The mountains\nare calling\nand I must go.\n— John Muir", QuoteTheme::SCENE},
{"Life begins at\nthe end of your\ncomfort zone.\n— Neale Donald\n  Walsch", QuoteTheme::SCENE},
{"There is no\nend to the\nadventures we can\nhave if only\nwe seek them\nwith our eyes open.\n— Jawaharlal Nehru", QuoteTheme::SCENE},
{"A good traveler\nhas no fixed\nplans and is not\nintent on\narriving.\n— Lao Tzu", QuoteTheme::SCENE},
{"It is better\nto travel well\nthan to arrive.\n— Buddha", QuoteTheme::SCENE},
{"Roam abroad\nin the world,\nand take fill\nof its beauties.\n— Longfellow", QuoteTheme::SCENE},
{"Man cannot\ndiscover\nnew oceans unless\nhe has courage\nto lose sight\nof the shore.\n— Andre Gide", QuoteTheme::SCENE},
{"Fill your life\nwith adventures,\nnot things. Have\nstories to tell,\nnot stuff\nto show.\n— Unknown", QuoteTheme::SCENE},
{"I haven't been\neverywhere,\nbut it's on\nmy list.\n— Susan Sontag", QuoteTheme::SCENE},
{"Take only\nmemories,\nleave only\nfootprints.\n— Chief Seattle", QuoteTheme::SCENE},
{"The best dreams\nhappen when\nyou're awake.\n— Cherie Gilderbloom", QuoteTheme::SCENE},
{"Go confidently\nin the direction\nof your dreams.\nLive the life\nyou have\nimagined.\n— Henry David\n  Thoreau", QuoteTheme::SCENE},
{"Every day is\na journey, and\nthe journey itself\nis home.\n— Basho", QuoteTheme::SCENE},
{"Home is behind,\nthe world ahead,\nand there are\nmany paths to\ntread.\n— J.R.R. Tolkien", QuoteTheme::SCENE},
{"The world is\nbig and I want\nto have a good\nlook at it\nbefore it gets\ndark.\n— John Muir", QuoteTheme::SCENE},
{"Broad, wholesome,\ncharitable views\ncannot be acquired\nby vegetating\nin one corner.\n— Mark Twain", QuoteTheme::SCENE},
{"I travel not\nto go anywhere,\nbut to go.\nI travel for\ntravel's sake.\n— Robert Louis\n  Stevenson", QuoteTheme::SCENE},
{"Once a year,\ngo someplace\nyou've never\nbeen before.\n— Dalai Lama", QuoteTheme::SCENE},
{"Jobs fill your\npockets, but\nadventures fill\nyour soul.\n— Jaime Lyn", QuoteTheme::SCENE},
};

// ── Static methods ───────────────────────────────────────

const Quote& AmbientPoetryMode::getQuote(int index) {
    int count = getQuoteCount();
    return QUOTES[index % count];
}

int AmbientPoetryMode::getQuoteCount() {
    return sizeof(QUOTES) / sizeof(QUOTES[0]);
}

uint32_t AmbientPoetryMode::hashQuote(const char* text) {
    // djb2 hash — fast, good distribution
    uint32_t hash = 5381;
    while (*text) {
        hash = ((hash << 5) + hash) + *text;
        text++;
    }
    return hash;
}

// ── Mode interface ───────────────────────────────────────

void AmbientPoetryMode::init(uint32_t seed) {
    _seed = seed;
    randomSeed(seed);
    _quoteIndex = random(0, getQuoteCount());
    _borderStyle = random(0, 4);
    Serial.printf("[Poetry] Quote: %d/%d, border: %d\n", _quoteIndex, getQuoteCount(), _borderStyle);
}

void AmbientPoetryMode::generate(AnimationEngine &engine) {
    engine.clear();

    // Draw decorative border (animated)
    switch (_borderStyle) {
        case 0: drawVineBorder(engine); break;
        case 1: drawGeometricBorder(engine); break;
        case 2: drawDotBorder(engine); break;
        default: drawLineBorder(engine); break;
    }
}

void AmbientPoetryMode::renderText(DisplayManager &dm, int startY, int height) {
    const Quote &q = getQuote(_quoteIndex);
    const char *quote = q.text;

    // Parse lines
    String text = String(quote);
    int lineCount = 1;
    for (size_t i = 0; i < text.length(); i++) {
        if (text[i] == '\n') lineCount++;
    }

    int lineHeight = 18;
    int totalHeight = lineCount * lineHeight;
    // Start text 14px below startY (separator is at startY+4, then 10px gap)
    int textAreaTop = startY + 14;
    int textAreaH = height - 14;
    int y = textAreaTop + (textAreaH - totalHeight) / 2 + lineHeight;

    int leftMargin = 20;
    int rightMargin = 20;

    dm.display.setFont(&FreeSerif9pt7b);
    dm.display.setTextColor(GxEPD_BLACK);

    // Thin separator at top of quote area
    dm.display.drawLine(leftMargin, startY + 4, SCREEN_W - rightMargin, startY + 4, GxEPD_BLACK);

    int lineIdx = 0;
    String line = "";
    for (size_t i = 0; i <= text.length(); i++) {
        if (i == text.length() || text[i] == '\n') {
            int16_t x1, y1;
            uint16_t tw, th;

            if (line.startsWith("—") || line.startsWith("  ")) {
                // Attribution — right-justified, same font
                dm.display.setFont(&FreeSerif9pt7b);
                dm.display.getTextBounds(line.c_str(), 0, 0, &x1, &y1, &tw, &th);
                dm.display.setCursor(SCREEN_W - rightMargin - tw, y);
                dm.display.print(line.c_str());
                y += lineHeight;
            } else {
                // Regular quote text — left-aligned with margin
                dm.display.setFont(&FreeSerif9pt7b);
                dm.display.setCursor(leftMargin, y);
                dm.display.print(line.c_str());
                y += lineHeight;
            }

            line = "";
            lineIdx++;
        } else {
            line += text[i];
        }
    }
}

// ── Border styles ────────────────────────────────────────

void AmbientPoetryMode::drawVineBorder(AnimationEngine &engine) {
    int artH = SCREEN_H - 30;
    for (int i = 0; i < 60; i++) {
        float t = i * 0.15f;
        int x = 5 + (int)(3 * sin(t * 2.0f));
        int y = 10 + i * (artH - 20) / 60;
        engine.pixel(x, y);
        if (i % 5 == 0) engine.circle(x + 3, y, 2);
    }
    for (int i = 0; i < 60; i++) {
        float t = i * 0.15f;
        int x = SCREEN_W - 6 + (int)(3 * sin(t * 2.0f + 1.0f));
        int y = 10 + i * (artH - 20) / 60;
        engine.pixel(x, y);
        if (i % 5 == 0) engine.circle(x - 3, y, 2);
    }
}

void AmbientPoetryMode::drawGeometricBorder(AnimationEngine &engine) {
    int artH = SCREEN_H - 30;
    for (int i = 0; i < 8; i++) {
        int y = 10 + i * (artH - 20) / 8;
        engine.rect(2, y, 6, 6);
        engine.rect(SCREEN_W - 8, y, 6, 6);
    }
    engine.line(5, 5, SCREEN_W - 5, 5);
    engine.line(5, artH - 5, SCREEN_W - 5, artH - 5);
}

void AmbientPoetryMode::drawDotBorder(AnimationEngine &engine) {
    int artH = SCREEN_H - 30;
    for (int i = 0; i < 30; i++) {
        int y = 10 + i * (artH - 20) / 30;
        engine.fillCircle(5, y, 1);
        engine.fillCircle(SCREEN_W - 6, y, 1);
    }
    for (int i = 0; i < 20; i++) {
        int x = 10 + i * (SCREEN_W - 20) / 20;
        engine.fillCircle(x, 5, 1);
        engine.fillCircle(x, artH - 6, 1);
    }
}

void AmbientPoetryMode::drawLineBorder(AnimationEngine &engine) {
    int artH = SCREEN_H - 30;
    engine.line(3, 3, SCREEN_W - 3, 3);
    engine.line(3, artH - 3, SCREEN_W - 3, artH - 3);
    engine.line(3, 3, 3, artH - 3);
    engine.line(SCREEN_W - 3, 3, SCREEN_W - 3, artH - 3);
    engine.line(6, 6, SCREEN_W - 6, 6);
    engine.line(6, artH - 6, SCREEN_W - 6, artH - 6);
    engine.line(6, 6, 6, artH - 6);
    engine.line(SCREEN_W - 6, 6, SCREEN_W - 6, artH - 6);
}
