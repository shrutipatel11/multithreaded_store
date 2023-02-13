Name : Shruti Patel
GTID# : 903710704

In this project, I build a store which does the following:
  1. Receives product query from users.
  2. Requests each vendor services to send their bid for the product.
  3. Sends the collection of bids back to the user.

I used the grpc helloworld example (https://github.com/grpc/grpc/tree/master/examples/cpp/helloworld) as my template to develop the project.

Implementation:
  We are given two protocol files store.proto (for communication between the user and the store) and vendor.proto (for communication between the store and the vendor).

  Store Implementation:
  1. The store service starts listening on an address passed as an argument.
  2. Whenever it gets a ProductQuery request from a client, it reads list of vendors from vendor_address.txt (passed as argument).
  3. It creates a task which creates a channel with each vendor for asynchronous communication.
  4. It adds the task to the threadpool worker queue.
  5. Upon receiving the lock, it communicates with the vendor (calls a function getItemBid() exposed by the vendor)
  6. It then returns a ProductReply with price and vendor_id from all the vendors to the client.

  Vendor Implementation:
  1. The vendor service exposes a function getItemBid() which takes the name of the item as an argument.
  2. It initialises the BidQuery request and initiates an RPC call .
  3. Upon completion of the RPC, it returns the BidReply response received to the store server.

  Threadpool Implementation:
  1. It sets the maximum number of threads given as an argument.
  2. It exposes a function to add the tasks created by the store server to the tasks queue.
  3. The threadpool allows each thread to operate in an infinite loop in its function, continually waiting for new tasks to grab and run.

Setup:
  1. Install Docker desktop
  2. Run the command "docker-compose up -d" in the root folder containing the docker-compose.yml file.
  3. When the docker engine is running, select the "open in VSCode" option.
  4. Select the "Reopen folder in Container" 

Compiling and Testing:
Execute the following commands to compile:
  1. mkdir -p cmake/build
  2. pushd cmake/build
  3. cmake ../..
  4. make -j4
Check in case of errros in compilation. 
If there are no errors while build, the executable files will be generated in cmake/build/bin folder.
Open three terminals and navigate to the bin folder.
On terminal 1, run ./run_vendors vendor_addresses.txt &
On terminal 2, run ./store vendor_addresses.txt <ip_to_listen_on> <max_threads_allowed>
On terminal 3, run ./run_tests <ip_that_server_listens_on> <max_threads_allowed>
