# Sidequest-Tracker
A program that easily tracks sidequest from video games.

## Usage:

#### Starting the Program:
* Currently, this program only runs in terminal. An application is in progress but is not usable yet.
* In the Sidequest-Tracker folder, use ./main [Base or Save .txt file]
* A folder (Base_Files) is given for base file storage and (Save_Files) is given for save file storage.
* All created save files will automatically be stored in the Save_Files folder.

#### The Main Menu:
* When asked which action to take, there are several options:
  * Every option can be chosen by entering the full word or the first letter (excluding story, which uses st).
1. help / h: Gets this list.
2. exit / e: Exits the program (does not save progress).
3. save / s: Allows the user to save their current progress.
4. story / st: Allows the user to change how far into the game's story this program uses.
5. quest / q: Allows the user to remove or reuse quests.
6. print / p: Allows the user to get information relating to uncompleted quests.

## Base File Requirements:
1. Quests must all begin with a title category.
2. All quest categories must include the category name, a space, and the information for that category.
* Example: Title: Sample Name
* Commas after the category name will not influence anything.
3. All story points added must include a dash before the title.
* Example: -Chapter One

## Save File Requirements:
1. Save files are only created within the program.
2. After the program creates a save file, it should not be manually modified.
3. For the save file to function correctly, no information should be added that includes ".." anywhere.

## History
5/29/20: This program is fully functional with bug fixes in progress.
