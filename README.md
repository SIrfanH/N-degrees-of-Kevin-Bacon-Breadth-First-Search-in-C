# N-degrees-of-Kevin-Bacon-Breadth-First-Search-in-C
Small World hypothesis, first introduced by Stanley Milgram and other researchers, put forward the idea that any two person in the world is connected by roughly six intermediary person that they know. Thus the idea of six degrees of separation was born and has influenced different cultures and communities. For instance the “Erdos Number” which describes degrees of separation of mathematicians from Paul Erdos via their publications and “Six degrees of Kevin Bacon” which is degree of separation of actors from Kevin Bacon via their movies.<br/>
<br/>
# Six Degrees of Kevin Bacon
The Kevin Bacon number of an actor is the minimum movie distance that the actor has from Kevin Bacon. The Kevin Bacon number for Kevin Bacon is zero. The Kevin Bacon number of Arnold Schwarzenegger is two, because he has played in the movie Scavenger Hunt with Stephen Furst and Stephen Furst has shared the movie Animal House with Kevin Bacon.<br/>
<br/>
The Kevin Bacon number of Sean Connery is 2 which is shown below:<br/>
<br/>
<p align="center">
<img src="https://user-images.githubusercontent.com/49107892/153054757-5bc11db1-2849-4573-ace9-e1b97d332026.png" width="500">
 </p>
<br/>

## Implemented Three Main Features using Breadth First Search Algorithm
### Number of Actor for Every Kevin Bacon Number
Lists the number of actors for every Kevin Bacon number from 0 – N in the given input file. If an actor is not in any subnetwork that is connected to Kevin Bacon then the Kevin Bacon number of the actor is infinity. It also lists the number of actors with Kevin Bacon number of infinity.<br/>
### Kevin Bacon Number of an Actor
For the given input file the program finds the Kevin Bacon number of any actor given by the user input and shows it in the following format:<br/>
> Brad Pitt’s Kevin Bacon number is 2<br/>
> Brad Pitt-Julia Roberts : Ocean's Eleven (2001)<br/>
> Julia Roberts – Kevin Bacon : Flatliners (1990)<br/>

### Saving The Network to a Table
Additionally the program saves the Network of connections from previously asked actors by the user in a table. So that if the user an already asked actors kevin bacon number then it directly accesses the network via the table.<br/>

### Interface
When the program is run the first input that is requested from the user is the relative path of the input file that contains the data for the actors and the movie. After the file is read into the memory the user has to choose three options. First option for listing every actors Kevin Bacon Number, Second option for looking for Kevin Bacon Number of a specific actor and the third option to exit the program.<br/>
<br/>
<p align="center">
<img src="https://user-images.githubusercontent.com/49107892/153057387-a5e7783a-ea71-4a92-b521-98c67c4ad4b2.png" width="800">
</p>

