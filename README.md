# React-Chat-Box
A simple Rect chat box made in MERN stack. Implements a repurposed C++ Tries for handling word filtering using Node-addon-api

A Barebones project of a web chat app. Frontend is implemented in React, while backend is handled in NodeJs with express server. MongoDB is used to hold data on words that
need to be filtered. An addon was implemented using a prefix tree (Tries) to handle filtering user input and catching any blacklisted words, which prevents the input from being sent. Communication is done over the websocket protocol, using the ws library in Node.

The app works locally, however it will needs some adjustments to work on a dedicated server on the internet. Typical addition of a .env is needed to handle Ports, database URI, and anything else needed.

A verrrry basic style has been implemented to get the ball rolling, so don't expect the UI to be pretty.

Featues that could be added in the future:
1) Improved user tracking: Right now, clients are just given a number that is incremented. A more robust user tracking (and maybe authentication) could be used

2) Extend word filtering: For now, the Tries acts simply as a black list. It could be possible to add more tags to words, such as the Twitch emote keywords converting to images. Also, the Tries C++ files are reused from an old project, and are very rough and basic. A more robust prefix tree system could be helpful

3) Bettur UI

4) Implementation of chat rooms, direcrted messages, etc

5) Logging chat within the server
