/*******************************************************************************
> CSpotify - 21T1 DP1091 Assignment 2
| test_cspotify.c
|
| zID: <YOUR-ZID-HERE>
| Name: <YOUR-NAME-HERE>
| Date: <DATE-HERE>
|
| Version 1.0.0: Assignment released.
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_cspotify.h"
#include "cspotify.h"
#include "capture.h"

/*********************************************
> Test Functions
| These tests are explained in test_cspotify.h
*********************************************/

/*********
> STAGE 1
*********/

// Test function for 'add_playlist'
void test_add_playlist(void) {

    // Test 1: Does add_playlist return SUCCESS and add 
    // when adding one Playlist with a valid name?
    Library testLibrary = create_library();

    int result = add_playlist(testLibrary, "Favourites");
    if (result != SUCCESS) {
        printf("DOES NOT MEET SPEC\n");
        return;
    }

    //char printText[MAX_LEN];
    // CAPTURE(print_library(testLibrary), printText, MAX_LEN);
    // if (!string_contains(printText, "Favourites")) {
    //     printf("DOES NOT MEET SPEC\n");
    //     return;
    // }

    // Test 2: Does add_playlist return ERROR_INVALID_INPUTS
    // and not add the playlist into the Library
    // when trying to add a Playlist with an invalid name?
    // TODO: Add your test for Test 2
    int result2 = add_playlist(testLibrary,"^^");
    if (result2 != ERROR_INVALID_INPUTS){
        printf("DOES NOT MEET SPEC\n");
        return;
    }

    // Test 3: ???
    // TODO: Add your own test, and explain it.

    printf("MEETS SPEC\n");
}

// Test function for 'rename_playlist'
void test_rename_playlist(void) {
    // Test 1: Does rename_playlist return ERROR_NOT_FOUND 
    // when trying to rename a playlist when the Library is empty
    Library testLibrary = create_library();

    int result = rename_playlist(testLibrary, "Favourites", "Dislikes");
    if (result != ERROR_NOT_FOUND) {
        printf("DOES NOT MEET SPEC\n");
        return;
    }

    // Test 2: Does rename_playlist successfully
    // rename a Playlist
    // when a valid new Playlist name is given to 
    // rename an existing Playlist in the Library?
    // TODO: Add your test for Test 2
    add_playlist(testLibrary,"Favourites");   
    int result2 = rename_playlist(testLibrary, "Favourites", "Dislikes");
    if (result != ERROR_NOT_FOUND) {
        printf("DOES NOT MEET SPEC\n");
        return;
    }

    

    // Test 3: ???
    // TODO: Add your own test, and explain it.

    printf("MEETS SPEC\n");
}


/*********
> STAGE 2
*********/

// Test function for 'add_track'
void test_add_track(void) {
    // Test 1: Does add_track successfully add 
    // multiple (more than 3 Tracks) Tracks 
    // to the Library?
    // TODO: Add your test for Test 1
    Library testLibrary = create_library();
    add_playlist(testLibrary,"Favourites");   
    int result = add_track(testLibrary,"Five Hunderd Miles","Justin",206,0);
    if (result != SUCCESS) {
        printf("DOES NOT MEET SPEC\n");
        return;
    }
    // Test 2: ???
    // TODO: Add your own test, and explain it.

    printf("MEETS SPEC\n");
}

// Test function for 'playlist_length'
void test_playlist_length(void) {
    // Test 1: Does playlist_length work for Tracks 
    // with lengths greater than 60 seconds?
    // TODO: Add your test for Test 1
    Library testLibrary = create_library();
    add_playlist(testLibrary,"Favourites");   
    add_track(testLibrary,"Five Hunderd Miles","Justin",206,0);
    add_track(testLibrary,"Sold out","Justin",306,0);
    int second = 0;
    int minitus = 0;
    playlist_length(testLibrary,&minitus,&second);
    if (!(second == 32 && minitus == 8)){
        printf("DOES NOT MEET SPEC\n");
    }


    // Test 2: ???
    // TODO: Add your own test, and explain it.

    printf("MEETS SPEC\n");
}


/*********
> STAGE 3
*********/

// Test function for 'delete_playlist'
void test_delete_track(void);
void test_delete_playlist(void) {
    // Test 1: Does delete_playlist work if
    // the selected Playlist is the first Playlist
    // in the Library?
    // TODO: Add your test for Test 1
    Library testLibrary = create_library();
    add_playlist(testLibrary,"Favourites");   
    delete_playlist(testLibrary);
    int res = rename_playlist(testLibrary,"Favourites","test");
    if (res != ERROR_NOT_FOUND){
        printf("DOES NOT MEET SPEC\n");
    }


    // Test 2: ???
    // TODO: Add your own test, and explain it.

    printf("MEETS SPEC\n");
}

//　Test function for 'delte_track'
void test_delete_track(void){

}


/*********
> STAGE 4
*********/

// Test function for 'soundex_search'
void test_soundex_search(void) {
    // Test 1: ???
    // TODO: Add your own test, and explain it.
    Library testLibrary = create_library();
    add_playlist(testLibrary,"Favourites");   
    add_track(testLibrary,"Five Hunderd Miles","Justin",206,0);
    add_track(testLibrary,"Sold out","Josten",306,0);
    printf("\n");
    soundex_search(testLibrary,"Justin");

    // Test 2: ???
    // TODO: Add your own test, and explain it.

    printf("MEETS SPEC\n");
}


/*********
> EXTRA
*********/


void test_cut_and_paste_track(int case_id);
void test_add_filtered_playlist(int case_id);
//  Your extra tests (Not worth marks)
void extra_tests(void) {
    int case_id =0;
    test_add_filtered_playlist(case_id++);
    test_cut_and_paste_track(case_id++);
    test_delete_track();
}

void test_cut_and_paste_track(int case_id){
    int res;
    Library testLibrary = create_library();
    add_playlist(testLibrary,"Favourites");   
    add_playlist(testLibrary,"Favourites2");   
    add_track(testLibrary,"Five Hunderd Miles","Justin",206,0);
    add_track(testLibrary,"Sold out","Josten",306,0);
    printf("Test cut_and_paste_track");
    res = cut_and_paste_track(testLibrary,"Five Hunderd Miles","Favourites2");
    if (res != SUCCESS){
        printf("DOES NOT MEET SPEC %d",case_id++);
    }
}

void test_add_filtered_playlist(int case_id){
    int res;
    Library testLibrary = create_library();
    add_playlist(testLibrary,"Favourites");   
    add_playlist(testLibrary,"Favourites2");   
    add_track(testLibrary,"Five Hunderd Miles","Justin",206,0);
    add_track(testLibrary,"Sold out","Josten",306,0);
    add_track(testLibrary,"only my railgun","Koria",306,0);
    printf("Test add_filtered_playlist");
    res = add_filtered_playlist(testLibrary,"Justin");
    if (res != SUCCESS){
        printf("DOES NOT MEET SPEC %d",case_id++);
    }
}

/*****************
> Helper Functions
*****************/

// Find the string 'needle' in 'haystack'
int string_contains(char *haystack, char *needle) {
    return strstr(haystack, needle) != NULL;
}
