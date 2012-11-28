#include "poMessageCenter.h"
#include "poObject.h"

namespace po {
    namespace MessageCenter {
        typedef struct {
            Object* sender;
            std::string message;
            Dictionary dict;
        } Message;
        
        typedef struct {
            Object* sender;
            Object* subscriber;
        } MessageSubscriber;
        
        namespace {
            std::map<std::string, std::vector<MessageSubscriber* > > subscribers;
            std::vector<Message* > messageQueue;
        }
        
        //------------------------------------------------------------------
        void update() {
            //Go through queue, broadcasting messages
            for(std::vector<Message*>::iterator mIter = messageQueue.begin(); mIter != messageQueue.end(); ++mIter) {
                Message* m = (*mIter);
                
                //Go through subscribers for this message, checking to see if they need to be alerted
                for (std::vector<MessageSubscriber* >::iterator sIter = subscribers[m->message].begin(); sIter != subscribers[m->message].end(); ++sIter) {
                    MessageSubscriber* thisSubscriber = (*sIter);
                    
                    if(thisSubscriber->sender == NULL || thisSubscriber->sender == m->sender) {
                        (*sIter)->subscriber->messageHandler(m->message, m->dict);
                    }
                }
                
                delete m; m=NULL;
            }
            
            messageQueue.clear();
        }
        
        
        //------------------------------------------------------------------
        void addSubscriber(std::string msg, Object* subscriber, Object* sender) {
            subscribers[msg].push_back(new MessageSubscriber());
            subscribers[msg].back()->sender       = sender;
            subscribers[msg].back()->subscriber   = subscriber;
        }
        
        
        //------------------------------------------------------------------
        void removeSubscriber(std::string msg, Object* subscriber) {
            if(subscribers.find(msg) != subscribers.end()) {
                if(!subscribers[msg].empty()) {
                    //Find the subscriber and delete it
                    for(std::vector<MessageSubscriber* >::iterator sIter = subscribers[msg].begin(); sIter != subscribers[msg].end(); ++sIter) {
                        MessageSubscriber* thisSubscriber = (*sIter);
                        if(thisSubscriber->subscriber == subscriber) {
                            delete (*sIter);
                            subscribers[msg].erase(sIter);
                        }
                    }
                }
            }
        }
        
        
        //------------------------------------------------------------------
        void removeAllSubscribers(std::string msg) {
            if(subscribers.find(msg) != subscribers.end()) {
                subscribers[msg].clear();
                
                std::map<std::string, std::vector<MessageSubscriber* > >::iterator it;
                it = subscribers.find(msg);
                subscribers.erase(it);
            }
        }
        
        
        //------------------------------------------------------------------
        void broadcastMessage(std::string msg, Object* sender, const Dictionary& dict) {
            if(subscribers.find(msg) != subscribers.end()) {
                messageQueue.push_back(new Message());
                
                messageQueue.back()->sender     = sender;
                messageQueue.back()->message    = msg;
                messageQueue.back()->dict       = dict;
            }
        }
    }
}
