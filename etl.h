#ifndef ETL_H_
#define ETL_H_

/*
 * physical data page structure :
 * --------------------
 * Erase Cycle          [2B]
 * Effective Erase Cyle [2B]
 * Logic Page           [1B/2B]
 * Hot                  [1B]
 * check sum            [1B]
 * data                 [8B?]
 * -------------------
 *
 * info page :
 * -------------------
 * logic page size      [1B]
 * tital page count     [2B]
 * thresh hold          [2B]
 * -------------------
 *
 * dual-pool algorithm :
 * Dirty Swap:
 *      MaxQue<EC,HP>.front - MaxQue<EC,CP>.back > TH
 *
 * Cold Pool Resize :
 *      MaxQue<EEC,CP>.front - MaxQue<EEC,HP>.back > TH
 *
 * Hot Pool Resize :
 *      MaxQue<EC,HP>.front - MaxQue<EC,HP>.back > 2*TH
 */




#endif
