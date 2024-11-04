# Data Structures Phase 1

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
![Alt-txt](/figues/Screenshot 2024-11-04 at 1.48.01 PM.png)











