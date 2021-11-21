/*
 * The main file for controlling the server. The main loop of the server goes roughly
 * like this:
 *
 * 1) Poll for any clients requesting to connect to the server on the server's input pipe
 * 2) Poll for any requests from the connected clients
 * 3) Distribute any events
 * 4) Repeat
*/


