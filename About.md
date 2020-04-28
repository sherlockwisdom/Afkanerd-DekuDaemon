#### About
This contains a simple "about" for how this messaging platform works

###### Terminologies:
- *Best Effort:* https://en.wikipedia.org/wiki/Best-effort_delivery

###### Findings
- Message delivery suffers once message is above 150 chars 
> code has been added to truncate every message into this scope
- Due to SMS messages being a _Best Effort_ delivery, this means registering 100% of SMS messages reaching their reciepients cannot be guaranteed
> Methods to counter this include
> - Using an algorithm for detecting false positives. The assumption is on total failure of a message, no following messages will be forwarded. Any deviation from this basic assumption can be considered *NOT A TOTAL FAILURE*
