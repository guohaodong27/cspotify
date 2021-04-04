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
    int padding;
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
int add_playlist(Library library, char playlistName[MAX_LEN]) {
    if (!is_valid(playlistName)){
        return ERROR_INVALID_INPUTS;
    }
    // init the playlist
    Playlist newPlaylist = malloc(sizeof(struct playlist));
    strcpy(newPlaylist->name,playlistName);
    newPlaylist->isSelected = FALSE;
    newPlaylist->next = NULL;
    newPlaylist->tracks = NULL;
    // append to the end of liberary
    if (library->head == NULL) {
        newPlaylist->isSelected = TRUE;
        library->selected = newPlaylist;
        library->head = newPlaylist;
    }else{
        library->selected =newPlaylist;
        Playlist cur = library->head;
        Playlist next = cur->next;
        while (next != NULL)
        {
            next = next->next;
        }
        
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
    if (library == NULL){
        return;
    }
    Playlist cur = library->head;
    int index = 0;
    while (cur!=NULL) {
        if (cur->isSelected) {
            print_selected_playlist(index,cur->name);
            Track curT = cur->tracks;
            while (curT != NULL)
            {
                print_track(curT->title,curT->artist,curT->length.minutes,curT->length.seconds);
                curT = curT->next;
            }
            
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
    while (cur != NULL && strcmp(cur->name,playlistName)) {
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
    while (playlist != NULL && !playlist->isSelected){
        playlist = playlist->next;
    }
    if (playlist == NULL){
        return;
    }
    playlist->isSelected = FALSE;
    // in case of no next playlist
    if (playlist->next == NULL){
        library->head->isSelected = TRUE;
        library->selected = library->head;
    }else{
        playlist->next->isSelected = TRUE;
        library->selected = playlist->next;
    }
}

// Selects the previous Playlist in the Library.
void select_previous_playlist(Library library) {
    // check Playlist is not null
    Playlist cur = library->head;
    if (cur == NULL) {
        return;
    }
    if(cur->isSelected){
        cur->isSelected = FALSE;
        library->last->isSelected = TRUE;
        library->selected = library->last;
    }
    Playlist next = cur->next;
    while (next != NULL && !next->isSelected){
        cur = next;
        next = next->next;
    }
    next->isSelected = FALSE;
    cur->isSelected = TRUE;
    library->selected = cur;
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
        // init track
        Track insertT = malloc(sizeof(struct track));
        strcpy(insertT->title,title);
        strcpy(insertT->artist,artist);
        insertT->next = NULL;
        struct trackLength len;
        len.seconds = trackLengthInSec%60;
        len.minutes = trackLengthInSec/60;
        insertT->length = len;
        // find last track
        Track cur = insterPosition->tracks;
        if (cur == NULL){
            insterPosition->tracks = insertT;
            return SUCCESS;
        }
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        cur->next = insertT;
    return SUCCESS;
}

// Calculate the total length of the selected Playlist in minutes and seconds.
void playlist_length(Library library, int *playlistMinutes, int *playlistSeconds) {
    *playlistMinutes = 0;
    *playlistSeconds = 0;
    Playlist cur = library->head;
    while (cur != NULL)
    {
        Track curT = cur->tracks;
        while (curT != NULL)
        {
            (*playlistMinutes)+= curT->length.minutes;
            *playlistSeconds+= curT->length.seconds;
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
            if (curT != NULL && !strcmp(curT->title,track)){
                cur->tracks = curT->next;
                free(curT);
                return;
            }
            Track pre = curT;
            curT = curT->next;
            while (curT != NULL)
            {
                if (!strcmp(curT->title,track))
                {
                    pre->next = curT->next;
                    free(curT);
                    return;
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
    if (cur != NULL && cur->isSelected){
        library->head = cur->next;
        library->selected = cur->next;
        Playlist newLast = library->head;
        while (newLast != NULL && newLast->next != NULL)
        {
            newLast = newLast->next;
        }
        library->last = newLast;
        free(cur);
        return;
    }
    Playlist pre = cur;
    cur = cur->next;
    while (cur != NULL && !cur->isSelected)
    {
        cur = cur->next;
    }
        // free mem
        if (cur->tracks != NULL){
            Track curT = cur->tracks;
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
        pre->next = cur->next;
        free(cur);
        // select next playlist
        if (cur != NULL){
            cur->isSelected = TRUE;
            library->selected = cur;
        }
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
        // find selected list
        while (curP != NULL && !curP->isSelected)
        {
            curP = curP->next;
        }
        Track curT = curP->tracks;
        Track preT = NULL;
        // find track
        while (curT!=NULL && strcmp(curT->title,trackTitle))
        {
            preT = curT;
            curT = curT->next;
        }
        if (curT == NULL){
            return ERROR_NOT_FOUND;
        }
        

        Playlist destP;
        destP = library->head;
        // find dest playlist
        while (destP!= NULL && strcmp(destP->name,destPlaylist))
        {
            destP= destP->next;
        }
        // not found destination
        if (destP== NULL){
            return ERROR_NOT_FOUND;
        }

        // del track
            // del first node
        if (curT != NULL && preT == NULL){
            curP->tracks = curT->next;
        }else{

            // del node which not at first
            preT->next = curT->next;
        }

        Track insertT = destP->tracks;
        if (insertT == NULL){
            curT->next = NULL;
            destP->tracks = curT;
            return SUCCESS;
        }
        while (insertT != NULL && insertT->next != NULL)
        {
            insertT = insertT->next;
        }
        curT->next = NULL;
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
                if(!strcmp(sound,s_artist)){
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
    // append_track record append position
    Track append_track = NULL;
    char sound[MAX_LEN];
        map(artist,sound);
        Playlist playlist = library->head;
        while (playlist->next != NULL)
        {
            Track track = playlist->tracks;
            Track pre = NULL;
            while (track != NULL)
            {
                char s_artist[MAX_LEN];
                map(track->artist,s_artist);
                if(!strcmp(s_artist,sound)){
                    if(library->last->tracks == NULL){
                        // add to playlist
                        library->last->tracks = track;
                        append_track = library->last->tracks;
                    }else{
                        append_track->next = track;
                    }
                }
                if (pre == NULL){
                    playlist->tracks = track->next;
                }else{
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
    Track order_node[MAX_LEN];
    Playlist curP = library->head;
    // in case of library is null
    if (curP == NULL){
        return;
    }
    while (curP != NULL && !curP->isSelected)
    {
        curP = curP->next;
    }

    // reorder nodes
    Track cur = curP->tracks;
    for(int i = 0;i< length;i++){
        if (cur == NULL){
            return ;
        }
        order_node[i] = cur;
        cur = cur->next;
    }
    order_node[length] = cur;

    // map order
    Track reordered_node[MAX_LEN];
    for(int i =0;i<length;i++){
        reordered_node[i] = order_node[order[i]];
    }
    // append last->next
    reordered_node[length] = order_node[length];

    // link ordered node
    library->selected->tracks = reordered_node[0];
    Track head = library->selected->tracks;
    for(int i = 1;i<length+1;i++){
        head->next = reordered_node[i];
        head = reordered_node[i];
    }
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



// map character to numver to realize soundex
static void map(char* i,char* res){
    char map[] = {'0','1','2','3','0','1','2','0','0','2','2','4','5','5','0','1','2','6','2','3','0','1','0','2','0','2'};

    int index = 0;
    
    // conver any character to lowcase
    char tmp_array[MAX_LEN];
    memset(tmp_array,0,MAX_LEN);
    while (i[index] != '\0')
    {
        if ('A'<=i[index]&&i[index]<='Z')
        {
            tmp_array[index]=i[index]+32;
        }else{
            tmp_array[index] = i[index];
        }
        index++;
    }

    // tmp_array index
    index = 0;
    // res index,because res's length is not eq to origin array
    int res_index =0;
    while (tmp_array[index] != '\0')
    {
        char tmp = map[tmp_array[index]-'a'+1];
        if(index == 0 || (index>0 && res[res_index-1] != tmp)){
            res[res_index] = tmp;
            res_index++;
        }
        index++;
    }
    res[index]='\0';
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
