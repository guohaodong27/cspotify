/*******************************************************************************
> CSpotify - 21T1 DP1091 Assignment 2
| cspotify.c
|
| zID: <YOUR-ZID-HERE>
| Name: <YOUR-NAME-HERE>
| Date: <DATE-HERE>
| Program Description:
| <INSERT DESCRIPTION>
|
| Version 1.0.0: Assignment released.
|
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cspotify.h"

/******************************************************************************/
// TODO: Add any other #defines you need.
#define TRUE 1
#define FALSE 0

/******************************************************************************/
// 'struct library' represents a library, which represents the state of the
// entire program. It is mainly used to point to a linked list of 
// playlists, though you may want to add other fields to it.
//
// You may choose to add or change fields in this struct.
struct library {
    struct playlist *head;
    struct playlist *last;
    struct playlist *selected;
};

// 'struct playlist' represents a playlist. 
// You may choose to add or change fields in this struct.
struct playlist {
    char name[MAX_LEN];
    struct track *tracks;
    struct playlist *next;
    int isSelected;
};

// 'struct trackLength' represents the length of a track. 
// You may choose to add or change fields in this struct.
struct trackLength {
    int minutes;
    int seconds;
};

// 'struct track' represents a track. 
// You may choose to add or change fields in this struct.
struct track {
    char title[MAX_LEN];
    char artist[MAX_LEN];
    struct trackLength length;
    struct track *next;
};

/******************************************************************************/
// TODO: Add any other structs you define here.


/******************************************************************************/
// TODO: Add prototypes for any extra functions you create here.

// Function prototypes for helper functions. 
static void print_playlist(int number, char playlistName[MAX_LEN]);
static void print_selected_playlist(int number, char playlistName[MAX_LEN]);
static void print_track(char title[MAX_LEN], char artist[MAX_LEN], int minutes, int seconds);
static void map(char i[MAX_LEN],char res[MAX_LEN]);
static int is_valid(char name[MAX_LEN]);

/******************************************************************************/
// You need to implement the following functions.
// In other words, write code to make the function work as described 
// in cspotify.h

/*********
> STAGE 1
*********/

// Create a new Library and return a pointer to it.
Library create_library(void) {
    Library newLibrary = malloc(sizeof(struct library));
    newLibrary->head = NULL;
    return newLibrary;
}

// Add a new Playlist to the Library.
// TODO: del
// 添加列表
int add_playlist(Library library, char playlistName[MAX_LEN]) {
    if (!is_valid(playlistName)){
        return ERROR_INVALID_INPUTS;
    }
    // init the playlist
    Playlist newPlaylist = malloc(sizeof(struct playlist));
    strcpy(newPlaylist->name,playlistName);
    newPlaylist->isSelected = FALSE;
    // append to the end of liberary
    if (library->head == NULL) {
        newPlaylist->isSelected = TRUE;
        library->selected = newPlaylist;
        library->head = newPlaylist;
    }else{
        newPlaylist->isSelected = FALSE;
        library->selected =newPlaylist;
        Playlist cur = library->head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = newPlaylist;
    }
    library->last=newPlaylist;
    return SUCCESS;
}

// Print out the Library.
void print_library(Library library) {
    Playlist cur = library->head;
    int index = 0;
    while (cur!=NULL) {
        if (cur->isSelected) {
            print_selected_playlist(index,cur->name);
        } else{
            print_playlist(index,cur->name);
        }
        cur = cur->next;
        index++;
    }
}

// Rename the name of an existing Playlist.
int rename_playlist(Library library, char playlistName[MAX_LEN],
    char newPlaylistName[MAX_LEN]) {
    Playlist cur = library->head;
    while (cur != NULL && !strcmp(cur->name,playlistName)) {
        cur = cur->next;
    }
    if (cur == NULL) {
        return ERROR_NOT_FOUND;
    }else{
        strcpy(cur->name,newPlaylistName);
    }
    return SUCCESS;
}


/*********
> STAGE 2
*********/

// Selects the next Playlist in the Library.
void select_next_playlist(Library library) {
    // check Playlist is not null
    Playlist playlist = library->head;
    while (playlist != NULL){
        if (playlist->isSelected) {
            playlist->isSelected = FALSE;
            playlist->next->isSelected = TRUE;
        }
        playlist = playlist->next;
    }
}

// Selects the previous Playlist in the Library.
void select_previous_playlist(Library library) {
    // check Playlist is not null
    Playlist cur = library->head;
    if (cur == NULL) {
        return;
    }
    Playlist next = cur->next;
    while (next != NULL){
        if (next->isSelected) {
            next->isSelected = FALSE;
            cur->isSelected = TRUE;
        }
        cur = next;
        next = next->next;
    }
}

// Add a new Track to the selected Playlist.
int add_track(Library library, char title[MAX_LEN], char artist[MAX_LEN], 
    int trackLengthInSec, int position) {
        // notice the playlist may be null
        Playlist insterPosition = library->head;
        // find playlist
        for(int i = 0;i<position;i++){
            insterPosition = insterPosition->next;
        }
        // find last track
        Track cur = insterPosition->tracks;
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        Track insertT = malloc(sizeof(struct track));
        strcpy(insertT->title,title);
        strcpy(insertT->artist,artist);
        struct trackLength len;
        len.seconds = trackLengthInSec;
        insertT->length = len;
        
    return SUCCESS;
}

// Calculate the total length of the selected Playlist in minutes and seconds.
void playlist_length(Library library, int *playlistMinutes, int *playlistSeconds) {
    Playlist cur = library->head;
    int i =0;
    while (cur->next != NULL)
    {
        Track curT = cur->tracks;
        while (curT != NULL)
        {
            playlistMinutes[i]+= curT->length.minutes;
            playlistSeconds[i]+= curT->length.seconds;
            curT=curT->next;
        }
        cur = cur->next;
    }
}


/*********
> STAGE 3
*********/

// Delete the first instance of the given track in the selected Playlist
// of the Library.
void delete_track(Library library, char track[MAX_LEN]) {
    Playlist cur = library->head;
    while (cur != NULL)
    {
        if (cur->isSelected)
        {
            Track curT = cur->tracks;
            while (curT != NULL)
            {
                if (strcmp(curT->title,track))
                {
                    Track tmp = curT;
                    curT=curT->next;
                    free(curT);
                }
                curT = curT->next;
            }
            
        }
        cur = cur->next;
        
    }
    

}

// Delete the selected Playlist and select the next Playlist in the Library.
void delete_playlist(Library library) {
Playlist cur = library->head;
    while (cur != NULL && !cur->isSelected)
    {
        cur = cur->next;
    }
        // delete selected playlist
        Playlist tmp = cur;
        cur = cur->next;
        // free mem
        if (tmp->tracks != NULL){
            Track curT = tmp->tracks;
            Track nextT = curT->next;
            while (curT != NULL)
            {
                free(curT);
                curT = nextT;
                if (nextT != NULL){
                    nextT = nextT->next;
                }
            }
        }
        
        free(tmp);
        // select next playlist
        cur->isSelected = TRUE;
}

// Delete an entire Library and its associated Playlists and Tracks.
void delete_library(Library library) {
    Playlist cur = library->head;
    while (cur != NULL )
    {
        Playlist tmp = cur;
        cur = cur->next;
        // free mem
        if (tmp->tracks != NULL){
            Track curT = tmp->tracks;
            Track nextT = curT->next;
            while (curT != NULL)
            {
                free(curT);
                curT = nextT;
                if (nextT != NULL){
                    nextT = nextT->next;
                }
            }
        }
        
        free(tmp);
        cur = cur->next;
    }
}


/*********
> STAGE 4
*********/

// Cut the given track in selected Playlist and paste it into the given 
// destination Playlist.
int cut_and_paste_track(Library library, char trackTitle[MAX_LEN], 
    char destPlaylist[MAX_LEN]) {
        Playlist curP = library->head;
        while (curP != NULL && !curP->isSelected)
        {
            curP = curP->next;
        }
        Track curT = curP->tracks;
        while (curT!=NULL && !strcmp(curT->title,trackTitle))
        {
            curT = curT->next;
        }
        curP = library->head;
        while (curP != NULL && !strcmp(curP->name,destPlaylist))
        {
            curP = curP->next;
        }
        Track insertT = curP->tracks;
        while (insertT != NULL && insertT->next == NULL)
        {
            insertT = insertT->next;
        }
        curT->next = insertT->next;
        insertT->next = curT;
    return SUCCESS;
}

// Print out all Tracks with artists that have the same Soundex Encoding 
// as the given artist.
void soundex_search(Library library, char artist[MAX_LEN]) {
        char sound[MAX_LEN];
        map(artist,sound);
        Playlist playlist = library->head;
        while (playlist != NULL)
        {
            Track track = playlist->tracks;
            while (track != NULL)
            {
                char s_artist[MAX_LEN];
                map(track->artist,s_artist);
                if(s_artist == sound){
                    print_track(track->title,track->artist,track->length.minutes,track->length.seconds);
                }
                track = track->next;
            }
            playlist = playlist->next;
        }
        
}


/*********
> STAGE 5
*********/

// Move all Tracks matching the Soundex encoding of the given artist 
// to a new Playlist.
int add_filtered_playlist(Library library, char artist[MAX_LEN]) {
    // add a new playlist to save Soundex
    add_playlist(library,artist);
    Track append_track;
    char sound[MAX_LEN];
        map(artist,sound);
        Playlist playlist = library->head;
        while (playlist != NULL)
        {
            Track track = playlist->tracks;
            Track pre;
            while (track != NULL)
            {
                char s_artist[MAX_LEN];
                map(track->artist,s_artist);
                if(s_artist == sound){
                    if(library->last->tracks == NULL){
                        // add to playlist
                        library->last->tracks = track;
                        append_track = library->last->tracks;
                    }
                    append_track->next = track;
                    // delete old track
                    pre->next = track->next;
                }
                pre = track;
                track = track->next;
            }
            playlist = playlist->next;
        }
    return SUCCESS;
}

// Reorder the selected Playlist in the given order specified by the order array.
void reorder_playlist(Library library, int order[MAX_LEN], int length) {
    
}


/*****************
> Helper Functions
*****************/

static void print_playlist(int number, char playlistName[MAX_LEN]) {
    printf("[ ] %d. %s\n", number, playlistName);
}

static void print_selected_playlist(int number, char playlistName[MAX_LEN]) {
    printf("[*] %d. %s\n", number, playlistName);
}

static void print_track(char title[MAX_LEN], char artist[MAX_LEN], int minutes, int seconds) {
    printf("       - %-32s    %-24s    %02d:%02d\n", title, artist, 
        minutes, seconds);
}



static void map(char i[MAX_LEN],char res[MAX_LEN]){
    char map[] = {'0','1','2','3','0','1','2','0','0','2','2','4','5','5','0','1','2','6','2','3','0','1','0','2','0','2'};
    int index = 0;
    while (i[index] != '\0')
    {
        if (i[index]<97)
        {
            i[index]=i[index]+31;
        }
        char tmp = map[i[index]-'a'];
        if(index-1>-1&& res[index-1] != tmp){
            res[index] = tmp;
        }
        index++;
    }
}

static int is_valid(char name[MAX_LEN]){
    int index = 0;
    while (name[index] != '\0' && index < MAX_LEN)
    {
        char cur = name[index];
        if(!((cur >= 'A' && cur <= 'Z')||(cur >='a' && cur <= 'z')||(cur >'0' && cur < '9'))){
            return FALSE;
        }
        index++;
    }
    return TRUE;
}
