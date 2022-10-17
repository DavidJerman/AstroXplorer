# AstroXplorer
AstroXplorer is an application that allows the user the view various data about the Earth and other celestial
 bodies in our solar system. The application consists of multiple tabs, each with its own topic. As such it 
 offers the following information:
 * Welcome (image)
 * Mars Rover Manifest
 * Mars Rover Imagery
 * EPIC Daily Blue Marble
 * Earth View
 * Podcasts

## Welcome image
The Welcome (image) tab is the default tab, that is opened/selected upon openning the application. It obtains
 the image of the day and displays it to the user along with the image's description and title, as well as the
 date of the image of the date - the date usually being the same as the date of opening the application. The
 image is obtained using the NASA APOD API and thus it takes some time for the image to load.

## Mars Rover Manifest
The Mars Rover Manifest contains information about the three Mars Rover: Curiosity, Opportunity and Spirit.
 Each of the rovers has information about the cameras that it has, maximum number of sols, when the mission has
 ended and started etc. This information is especially useful when using the Mars Rover Imagery tab to obtain
 the rover imagery, as that tab requires the user to specify the date(s) of images, that the user wants to view.
 The API used here is the Mars Rover Imagery API, also provided by NASA.

## Mars Rover Imagery
This tab allows the user to explore the martian rovers' imagery. The user can either browse the photos by a
 specific date or sol (martian day) and obtain and view images for a specific date. The user can also download
 images using the download tab, by selecting the cameras and rovers, as well as specifying what period of time
 the images should be from. This uses the same API as the Mars Rover Manifest tab.

## EPIC Daily Blue Marble
This tab allows the user to obtain images of the Earth captured by the DSCOVR's EPIC imagery instrument. The
 user can either type in the date - though it is to be noted, that some days have no imagery - or can use the
 provided slider to choose from the dates that do contain imagery. Two types of images are available: normal
 and enchanced. The tab also displays information about each image. Usually each date contains multiple images,
 taken at different times of the day and using the play button, the user can, for most dates, view the animation
 of a rotating Earth. The API used here is the EPIC API, provided by NASA.

## Earth View
This is a yet uncomplete tab, however it is going to function similar to Google Maps, except there won't be any
 advanced features such as route planning, markers etc., however the user will be able to choose among various
 different maps depicting everyhting from Earth temperatures, wind speeds to the Earth's relief. The map tiles
 are loaded on the run and when needed, so the user does not have to load the entire map to view it. The tab
 uses the GIBS API provided by NASA.

## Podcasts
This tab allows the user to play podacasts, that can be loaded from RSS files. It works with any RSS file that
 is properly formatted and has all the audio controls that one would expect - volume control, next/previous song,
 skip time, save as favorite, search podcast/episode, autplay etc. This tab does not use an API, since it 
 obtains all the data using the provided RSS files - each file representing an episode.

## TODO:
- prevent user from multiple image requests
- add keyboard events to control certain parts of application