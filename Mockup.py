import sys
import random
import math

#the length of the entire message that all clients will send
maxlength = 0

def xor_strings(s,t):
    return "".join(chr(ord(a)^ord(b)) for a,b in zip(s,t))

#assume each client gets a single character slot and that msg and slotNum are 0-indexed
#msg is the client's single character message for the round
#slotMax is length of entire message
def insert_in_slot(msg, slotNum, slotMax):
    runningString = ""
    for x in range(0, slotNum):
        runningString += "\0"
    runningString += msg
    for x in range(0, slotMax - slotNum):
        runningString += "\0"
    return runningString

#xor any strings that are not the empty string
def xor_all_strings(*strs):
    global maxlength
    #construct a null string big enough to xor into messages
    running_s = ""
    #just assuming that strings are at most length 10
    for i in range(0, maxlength):
        running_s += "\0"
    
    for s in strs:
        if len(s) != 0:
            #print s, len(s)
            running_s = xor_strings(s, running_s)
        else:
            pass
            #print "Whoa, problem"
    return running_s

#when client encrypts shared keys with message
#normally should be hashed shared keys, but shouldn't make any difference for this mockup
def encrypt(msg, client_num, view, secrets):
    for serv_num in view:
        msg = xor_strings(msg, secrets[serv_num][client_num])
    return msg

#when an individual server xors all ciphertexts with their shared keys
def decrypt_part1(server_num, cli_states, secrets, *msgs):
    msg = xor_all_strings(*msgs)
    counter = 0
    for cli_state in cli_states:
        if cli_state:
            msg = xor_strings(msg, secrets[server_num][counter])
        counter += 1
    return msg

#assume serv_cips in order
def get_cleartext(view, *serv_cips):
    global maxlength
    #construct a null string big enough to xor into messages
    running_s = ""
    #just assuming that strings are at most length 10
    for i in range(0, maxlength):
        running_s += "\0"
    
    counter = 0
    for cip in serv_cips:
        if counter in view:
            running_s = xor_strings(cip, running_s)
        counter += 1
    return running_s

#Assigns a particular clients message, represented by msg (this msg already has it in the proper slot)
# to a server that is in the view
def assign_server(msg, view, serv_msgs):
    index = random.choice(view)
    serv_msgs[index] = serv_msgs[index] + (msg,)
    return serv_msgs
   
#accepts an array of servers to be off and the server states
#and turns the appropriate servers off 
def manage_servers(state_str, serv_states):
    for i in range(0,len(serv_states)):
        serv_states[i] = True
    for state in state_str:
        if state != "":
            serv_states[int(state)] = False
    return serv_states

#same as manage_servers but for clients
def manage_clients(state_str, cli_states):
    for i in range(0,len(cli_states)):
        cli_states[i] = True
    for state in state_str:
        if state != "":
            cli_states[int(state)] = False
    return cli_states

#Randomly generates a shared key
def gen_secret(sec_length):
    running_sec = ""
    for i in range(0,sec_length):
        running_sec += str(chr(random.randrange(97, 97 + 26 + 1)))
    return running_sec
    
#Randomly generates all shared keys  
def gen_secrets(s_count, c_count):
    secrets = []
    for i in range(0, s_count):
        secrets.append([])
        for j in range(0, c_count):
            secrets[i].append(gen_secret(c_count))
    return secrets

#A recursive function to help get all of the views
def gen_all_views_helper(s_count, loop_num, target_size, all_combs):
    #if the lists are at the target size, then terminate
    if loop_num == target_size:
        return all_combs
    
    new_combs = []
    
    #take each of the old combinations and try adding a new element, and also only if the combination
    #is not already in the list
    for comb in all_combs:
        for i in range(0, s_count):
            if not i in comb:
                copy = list(comb)
                copy.append(i)
                copy.sort()
                if copy not in new_combs:
                    new_combs.append(copy)
    return gen_all_views_helper(s_count, loop_num + 1, target_size, new_combs)            
    
    
#Returns all possible views (each view will have 2/3 the number of servers)
def gen_all_views(s_count):
    return gen_all_views_helper(s_count, 0, int(math.ceil(2.0 / 3.0 * s_count)), [[]])

#need to make sure that every server in the view is on 
def check_view(view, serv_states):
    for serv in view:
        if not serv_states[serv]:
            return False
    return True

#Described in Dissent paper, all of the things the client does before and including passing
# the message to the server
def client_protocol(client_states, c_count, current_view, shared_secrets, server_msgs):
    #Intermediate variables
    var = []
    msg = []
    cip = []
    
    for i in range(0,c_count):
        var.append("")
        msg.append("")
        cip.append("")
    
    for i in range(0,c_count):
        if not client_states[i]:
            continue
        var[i] = raw_input("Message for client " + str(i) + ": ")
        msg[i] = insert_in_slot(var[i][0:1], i, c_count)
        cip[i] = encrypt(msg[i], i, current_view, shared_secrets)
        server_msgs = assign_server(cip[i], current_view, server_msgs)
    print server_msgs
    return server_msgs


#####MAIN#####
random.seed()
view_num = 0
s_count = int(raw_input("Number of servers: "))
c_count = int(raw_input("Number of clients: "))
maxlength = c_count   #length of messages
all_views = gen_all_views(s_count)

current_view = all_views[0]
print current_view

#indexed by server#, client#
shared_secrets = gen_secrets(s_count, c_count) 

while True:
    server_states = []
    client_states = []
    server_msgs = []
    
    #Leads to final output
    serv_cip = []
    
    for i in range(0,s_count):
        server_states.append(True)
        server_msgs.append(tuple())
    
    for i in range(0,c_count):
        client_states.append(True)

    #Just an initialization of the state
    s_states_str = raw_input("Servers off (0-index): ").split(" ")
    c_states_str = raw_input("Clients off (0-index): ").split(" ")
    server_states = manage_servers(s_states_str, server_states)
    client_states = manage_clients(c_states_str, client_states)
    print server_states, client_states
    
    #Nothing is done here yet, but what SHOULD happen is clients should request the current view
    
    #This is where the clients encrypt their messages using the current_view
    server_msgs = client_protocol(client_states, c_count, current_view, shared_secrets, server_msgs)
    
    #if the view is no good, then have the clients re-do the sending of the messages
    while not check_view(current_view, server_states):
        print "bad view", current_view
        view_num += 1
        current_view = all_views[view_num % len(all_views)]
        
        #clear out the assignments
        server_msgs = []
        for i in range(0,s_count):
            server_msgs.append(tuple())
        
        print "new view", current_view
            
        #Try again with another view, and this simulates telling the clients what the new view is
        server_msgs = client_protocol(client_states, c_count, current_view, shared_secrets, server_msgs)
    
    #Generating the server ciphertext
    for i in range(0,s_count):
        serv_cip.append(decrypt_part1(i, client_states, shared_secrets, *server_msgs[i]))

    #All client's messages put together
    decrypted = get_cleartext(current_view, *tuple(serv_cip))
    print decrypted
    
