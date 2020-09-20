#include "Room.h"
#include <iostream>

using namespace std;

void menu();
void look(Room* current_room);
void exits(Room* current_room);
void add(Room* current, vector<Room*>& allRooms);
void connect(Room* current, vector<Room*> allRooms);
void rename(Room* current);
void description(Room* current);
Direction char2Direction(char c); // convert a char value ('N','S','E','W')
                                  // to an enum value NORTH, SOUTH, etc

int main()
{
    // initialize
    Room* current_room = new Room();
    vector<Room*> allRooms;
    allRooms.push_back(current_room);
    char choice;

    // menu loop
    do
    {
        cout<<"Enter a command (Enter '?' to view list): ?"<<endl;
        cin>>choice;
        switch(choice)
        {
            case 'n':
                if (current_room->north()!= NULL)
                    current_room = current_room->north();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'e':
                if (current_room->east()!= NULL)
                    current_room = current_room->east();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 's':
                if (current_room->south()!= NULL)
                    current_room = current_room->south();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'w':
                if (current_room->west()!= NULL)
                    current_room = current_room->west();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'l':
                look(current_room); break;
            case 'x':
                exits(current_room);break;
            case 'c':
                connect(current_room, allRooms); break;
            case 'r':
                rename(current_room); break;
            case 'd':
                description(current_room); break;
            case 'a':
                add(current_room, allRooms); break;
            case 't':
                if(allRooms.size() > 1)
                    {
                    char temp;
                    int loc;
                    
                    cout << "What room would you like to teleport to?" << endl;
                    cout << "Type the NUMBER next to the desired room" << endl;
                    
                    for(int k = 0; k < allRooms.size(); k++){
                        cout << "\t" << k+1 << ". " << (allRooms.at(k))->name() << endl;
                    }
                    cin >> loc;
                    loc = loc-1;
                    
                    current_room = allRooms[loc];
                        
                    }
                break;
            case '?':
                menu(); break;
            case 'q':
                cout << "Quitting, all rooms will be destroyed." << endl; break;
            default:
                cout<<"invalid command"<<endl;
        }
    }while(choice != 'q');

    // cleanup before quitting
    for(int i = 0; i < allRooms.size(); i++){
        allRooms[i]->~Room();
    }

    return 0;
}

// Show the name and description of a room */
void look(Room* r)
{
    cout << "The current room is " << *r << endl;
}

// add a new room, connected to the current room
//  Parameters
//     current -- the current room we are in
//     allRooms-- a vector of pointers to (addresses of) all the rooms in the game
//  Precondition
//     current refers to an existing room
//  Postcondition
//    if possible (current must have a free exit)
//     a) a new room has been added off one of the exits of current
//     b) the address of the new room has also been added to the allRooms vector
void add(Room* current, vector<Room*>& allRooms)
{
    //Code checking an exit is available
    if(current->north() == NULL || current->south() == NULL ||
    current->east() == NULL || current->west() == NULL)
    {
        string name;
        string text;
        bool keep;
        char temp;
        Direction exit1;
        Direction exit2;
        
        cout << "Enter a name for the room and hit <enter>" << endl;
        cin.ignore(1, '\n');
        getline(cin, name);
        cout <<"Enter a description for this room and hit <enter>" <<endl;
        cin.ignore(1,'\n');
        getline(cin,text);
        
        Room* temp_room = new Room(name, text);
        allRooms.push_back(temp_room);
        
        //Code connecting new room to current room
        cout << endl << "What exit from " << current->name() << " do you want to use?" << endl;
        cout << "Type 'N' for North, 'E' for East, 'S' for South, or 'W' for West" << endl;
        cin >> temp;
        //Code checking exit choice is valid
        if(temp == 'N' && current->north() != NULL){
            temp = 'I';
        }
        else if(temp == 'E' && current->east() != NULL){
            temp = 'I';
        }
        else if(temp == 'S' && current->south() != NULL){
            temp = 'I';
        }
        else if(temp == 'W' && current->west() != NULL){
            temp = 'I';
        }
        else{}
        
        if(temp == 'N' || temp == 'E' || temp == 'S' || temp == 'W'){
            exit1 = char2Direction(temp);
            
            cout << "What exit from " << temp_room->name() << " do you want to use?" << endl;
            cout << "Type 'N' for North, 'E' for East, 'S' for South, or 'W' for West" << endl;
            cin >> temp;
            
            if(temp == 'N' && temp_room->north() != NULL){
                temp = 'I';
            }
            else if(temp == 'E' && temp_room->east() != NULL){
                temp = 'I';
            }
            else if(temp == 'S' && temp_room->south() != NULL){
                temp = 'I';
            }
            else if(temp == 'W' && temp_room->west() != NULL){
                temp = 'I';
            }
            else{}
            
            if(temp == 'N' || temp == 'E' || temp == 'S' || temp == 'W'){
                exit2 = char2Direction(temp);
                keep = current->connect(exit1, temp_room, exit2);
                
                if(keep == false){
                    allRooms.pop_back();
                    temp_room->~Room();
                }
            }
            else{
                cout << "This exit is unavailable." << endl;
                allRooms.pop_back();
                temp_room->~Room();
                
            }
            
        }
        else{
            cout << "This exit is unavailable." << endl;
            allRooms.pop_back();
            temp_room->~Room();
            
        }
        
        
    }
    else{
        cout << current->name() << " has no available exits!" << endl;
        cout << "You cannot attach a room here!" << endl;
    }
}

// add a new connection between current room and an already existing room.
//  Parameters
//     current -- the current room we are in
//     allRooms-- a vector of pointers to (addresses of) all the rooms in the game
//  Precondition
//     current refers to an existing room
//  Postcondition
//    if possible (both rooms must have a spare exit to link them together)
//     current is now linked to another room in the game
//
void connect(Room* current, vector<Room*> allRooms)
{
    if(current->north() == NULL || current->south() == NULL ||
    current->east() == NULL || current->west() == NULL)
    {
        
        char temp;
        int loc;
        Room* temp_room;
        Direction exit1;
        Direction exit2;
        
        cout << "What room would you like to connect to " << current->name() << "?" << endl;
        cout << "Type the NUMBER next to the desired room" << endl;
        
        for(int k = 0; k < allRooms.size(); k++){
            cout << "\t" << k+1 << ". " << (allRooms.at(k))->name() << endl;
        }
        cin >> loc;
        loc = loc-1;
        
        if(loc < allRooms.size()){
            
            temp_room = allRooms[loc];
            
            if(temp_room->north() == NULL || temp_room->south() == NULL ||
                temp_room->east() == NULL || temp_room->west() == NULL){
                //Code connecting new room to current room
                cout << endl << "What exit from " << current->name() << " do you want to use?" << endl;
                cout << "Type 'N' for North, 'E' for East, 'S' for South, or 'W' for West" << endl;
                cin >> temp;
                
                if(temp == 'N' && current->north() != NULL){
                    temp = 'I';
                    cout << "This exit is unavailable." << endl;
                }
                else if(temp == 'E' && current->east() != NULL){
                    temp = 'I';
                    cout << "This exit is unavailable." << endl;
                }
                else if(temp == 'S' && current->south() != NULL){
                    temp = 'I';
                    cout << "This exit is unavailable." << endl;
                }
                else if(temp == 'W' && current->west() != NULL){
                    temp = 'I';
                    cout << "This exit is unavailable." << endl;
                }
                else{}
                
                if(temp == 'N' || temp == 'E' || temp == 'S' || temp == 'W'){
                    exit1 = char2Direction(temp);
                    cout << "What exit from " << temp_room->name() << " do you want to use?" << endl;
                    cout << "Type 'N' for North, 'E' for East, 'S' for South, or 'W' for West" << endl;
                    cin >> temp;
                    
                    if(temp == 'N' && temp_room->north() != NULL){
                        temp = 'I';
                        
                        cout << "This exit is unavailable." << endl;
                    }
                    else if(temp == 'E' && temp_room->east() != NULL){
                        temp = 'I';
                        
                        cout << "This exit is unavailable." << endl;
                    }
                    else if(temp == 'S' && temp_room->south() != NULL){
                        temp = 'I';
                        
                        cout << "This exit is unavailable." << endl;
                    }
                    else if(temp == 'W' && temp_room->west() != NULL){
                        temp = 'I';
                        
                        cout << "This exit is unavailable." << endl;
                    }
                    else{}
                    
                    if(temp == 'N' || temp == 'E' || temp == 'S' || temp == 'W'){
                        exit2 = char2Direction(temp);
                        current->connect(exit1, temp_room, exit2);
                
                    }
                    else{
                        cout << "That's not valid input!" << endl;
                    }
                    
                }
                else{
                    cout << "That's not valid input!" << endl;
                }
            }
            
            else{
                cout << temp_room->name() << " has no available exits!" << endl;
                cout << "You cannot attach a room here!" << endl;
            }
            
        }
    }
    else{
        cout << current->name() << " has no available exits!" << endl;
        cout << "You cannot attach a room here!" << endl;
    }
        
    
}

// change the name of a room
void rename(Room* current)
{
    string name;
    cout << "Enter a new name for the room and hit <enter>" << endl;
    cin.ignore(1, '\n');
    getline(cin, name);
    current->set_name(name);
}

// change the description of a room
void description(Room* current)
{
    string text;
    cout<<"Enter a new description for this room and hit <enter>"<<endl;
    cin.ignore(1,'\n');
    getline(cin,text);
    current->set_description(text);
}

// for each exit from room r,
//   show the name of the room it connects to
//   or say "Nothing" if the link is null
void exits(Room* r)
{
    Room* nExit = r->north();
    Room* sExit = r->south();
    Room* eExit = r->east();
    Room* wExit = r->west();
    
    cout << (r->name()) << " Exits: " << endl;
    if(nExit == NULL){
        cout << "\tNorth: No exit" << endl;
    }
    else{
        cout << "\tNorth: " << nExit->name() << endl; 
    }
    if(sExit == NULL){
        cout << "\tSouth: No exit" << endl;
    }
    else{
        cout << "\tSouth: " << sExit->name() << endl; 
    }
    if(eExit == NULL){
        cout << "\tEast: No exit" << endl;
    }
    else{
        cout << "\tEast: " << eExit->name() << endl; 
    }
    if(wExit == NULL){
        cout << "\tWest: No exit" << endl;
    }
    else{
        cout << "\tWest: " << wExit->name() << endl; 
    }
    
}
void menu()
{
    cout<<"Please choose from the following: "<<endl;
    cout<<"     n or north --- move north (if possible)"<<endl;
    cout<<"      e or east --- move east (if possible)"<<endl;
    cout<<"     s or south --- move south (if possible)"<<endl;
    cout<<"      w or west --- move at west (if possible)"<<endl;
    cout<<"      l or look --- look at room"<<endl;
    cout<<"     x or exits --- show room exits"<<endl;
    cout<<"   c or connect --- connect this room to another (already made) room"<<endl;
    cout<<"    r or rename --- rename this room"<<endl;
    cout<<"      d or desc --- change description for this room"<<endl;
    cout<<"       a or add --- add new room"<<endl;
    cout<<"  t or teleport --- teleport between rooms"<<endl;
    cout<<"      q or quit --- exit program"<<endl;
    cout<<"              ? --- show commands (DUH)"<<endl;
}

// convert a char direction: 'N','S','E','W'
// into a Direction enum: NORTH, SOUTH, EAST, WEST
Direction char2Direction(char c)
{
    switch(c)
    {
        case 'N': return NORTH;
        case 'S': return SOUTH;
        case 'E': return EAST;
        case 'W': return WEST;
        default: return NORTH;
    }
}
