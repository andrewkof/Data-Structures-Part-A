# Data Structures Part A

## General Description
In this project, Ι implement a simplified movie streaming service. The service offers movies organized into different thematic categories. Users can register on the platform, watch movies 
and add them to their watch history, receive movie recommendations based on the watch history of other users, and perform filtered searches across movie categories.

## Detailed Description of the Implementation

The service you will implement organizes its available movies into six thematic categories: Horror, Science Fiction, Drama, Romance, Documentary, and Comedy. Each movie belongs to only one 
category and has a unique identifier. The categorization of movies will be implemented using a six-position array, called the category array. Each position in the array contains a pointer 
(of type ```struct movie *```) pointing to the first element of the linked list of movies for that particular category. This list is a singly linked list, sorted in ascending order based on the 
movie identifier (```mid```). A node in this list represents a movie within the category, defined by a structure (```struct movie```) with the following fields:

- **info:** A helper structure of type ```struct movie_info``` that describes the available information for a movie. Its fields are as follows:
   - **mid:** A unique identifier for the movie, of type ```unsigned int```.
   - **year:** The release year of the movie, of type ```unsigned int```.
- **next:** A pointer (of type ```struct movie```) that points to the next element in the movie list for the category.

Before being added to the appropriate list in the category array, new movies added to the service are first inserted into a separate list, the **new releases list**. This list contains 
movies from different categories, is a **singly linked list**, and is **sorted** in **ascending order** based on the movie identifier, just like the lists in the category array. The nodes 
in this list are implemented using the ```struct new_movie``` structure, which has the following fields:

- **info:** Information about the movie, of type ```struct movie_info```, as in ```struct movie```.
- **category:** The category to which this movie belongs, represented as an enum of type ```movieCategory_t```.
- **next:** A pointer (of type ```struct new_movie```) that points to the next node in the new releases list.

Figure 1 shows the category array in a hypothetical execution of the program. It is a fixed-size array with 6 positions, each containing a list of movies.

![Alt-txt](figures/Figure1.png)

The sentinel node of the user list is a node used for the correct management of the list (and is therefore an auxiliary node). It is of type ```struct user```, but its ```uid``` is set to 
-1. The pointers ```suggestedHead```, ```suggestedTail```, ```watchHistory```, and ```next``` for each user are initially set to ```NULL```. The possible structure of the user list in a 
hypothetical program execution is shown in Figure 2.

![Alt-txt](figures/Figure2.png)


To implement the doubly linked list of recommended movies for the user, we use the ```struct suggested_movie``` structure, which has the following fields:
- **info:** A field of type ```struct movie_info```, which contains information about the movie.
- **prev:** A pointer of type ```struct suggested_movie```, which points to the previous element in the recommended movies list.
- **next:** A pointer of type ```struct suggested_movie```, which points to the next element in the recommended movies list.

# Program Operation
The program you will implement should be executed with the following command: 
```<executable> <input file>```
where ```<executable>``` is the name of the executable (e.g., ```cs240StreamingService```) and ```<input file>``` is the name of an input file (e.g., testFile) a file that contains one event per line. The events need to be implemented in your program and follow one of the following formats:

## Event R (R `<uid>`)

A **Register User** event, in which a new user with identifier ```uid``` registers in the service. We first need to traverse the user list to check that there is no existing user with 
the identifier ```uid``` in the service. We then implement an insertion algorithm to add the new user to the user list. Our algorithm has a time complexity of O(1). The fields 
```suggestedHead```, ```suggestedTail```, and ```watchHistory``` for the new user are initialized to ```NULL```. After executing this event, the program prints the following information:

![Alt-txt](figures/R_Event.png)

where `n` is the number of nodes in the user list, and for each i ∈ {1, …, n}, `<uid_i>` is the identifier of the user corresponding to the i-th node in the list.

## Event U (U `<uid>`)

An **Unregister User** event, in which the user with identifier <uid> leaves the service. We first need to clear the user’s doubly linked list of recommended movies and their watch history 
stack by removing all their elements (if they exist) before removing the user from the list. After executing such an event, the program prints the following information:

![Alt-txt](figures/U_Event.png)

where `n` is the number of nodes in the user list, and for each i ∈ {1, …, n}, `<uid_i>` is the identifier of the user corresponding to the i-th node in the list.

## Event A (A `<mid> <category> <year>`)

An **Add New Movie** event, in which a movie with identifier `<mid>`, category `<category>`, and release year `<year>` is added to the service. We insert the movie into the sorted 
new releases list, not the category array list. The new releases list should remain sorted (in ascending order based on the movie identifier) after each insertion. After executing this 
event, the program prints the following information:

![Alt-txt](figures/A_Event.png)

where `n` is the number of elements in the new releases list, and `mid_i`, `category_i`, `year_i`, \( i \in \{1, …, n\} \), represent the identifier, category, and release year of the movie corresponding to the \( i \)-th node in the new releases list.


## Event D

A **Distribute New Movies** event, in which the movies from the new releases list are assigned to the lists in the category array based on their category. This event is implemented in 
time complexity of \( O(n) \), where `n` is the size of the new releases list. We need to traverse the new releases list once, removing each movie encountered and adding it to the 
appropriate list in the category array. By the end of this process, the new releases list should be empty, and the lists in the category array should be sorted in ascending order based on 
the movie identifier. After executing this event, the program prints the following information:

![Alt-txt](figures/D_Event.png)

where `n1`, `n2`, …, `n6` are the sizes of the six category lists, and `<mid_i,j>` is the identifier of the movie corresponding to the \( j \)-th node in the movie list for category \( i \).


## Event W (W `<uid> <mid>`)

A **User Watches Movie** event, in which the user with identifier `<uid>` watches the movie with identifier `<mid>`. First, we locate the movie information (`struct movie_info`) 
by searching through the lists in the category array, as well as locate the user by searching the user list. Next, we create a new `struct movie` and push it onto the user’s watch history 
stack. After executing this event, the program prints the following information:

![Alt-txt](figures/W_Event.png)

where `n` is the number of elements in the watch history stack of the user `<uid>`, and `mid_i`, \( i \in \{1, …, n\} \), is the identifier of the movie corresponding to the \( i \)-th 
element in the stack.














