This is a program to access the FRC (FIRST robotics competition) events API.  See https://frcevents2.docs.apiary.io/.  

This follows a similar style to an interface to The Blue Alliance's API implemented here: https://github.com/1425/tba

Note that the FRC events API requires being given a key before it will allow access; without that this program is not very useful.  Note that they do continue to give out the keys although the process is not instantaneous.  If you're in a hurry and the same data is available, you may be better off using the API of The Blue Alliance.

# PREREQUISITES
  * libcurl; tested with version 8.5.0
  * sqlite; tested with version 3.45.1
  * simdjson; tested with version 3.6.4
  * A C++20 compiler; tested with gcc 13.2.0

# LICENSE
GPL version 3.

# AUTHOR
Eric Rippey (rippey.e AT gmail.com; Softwarebug2.0 on ChiefDelphi)

# BUGS
The date-based cache updating is not yet implemented.
