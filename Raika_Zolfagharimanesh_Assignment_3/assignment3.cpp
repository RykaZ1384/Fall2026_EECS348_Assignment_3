/*
Program: EECS 348 Assignment 3
Description: C++ program that uses a MaxHeap to prioritize emails for a CEO.
Input: A test file with EMAIL, NEXT, READ, and COUNT commands.
Output: Shows the next email and the number of unread emails.
Collaborators: None
Other Sources: Copilot and Grok and Chatgpt to make sure I checked every possible input.
Author: Raika Zolfaghari
Creation Date: October 1, 2026
Revision Date: October 1, 2026
Revisions: Improved the Copilot code and added comments.
*/

#include <iostream> // for input and output
#include <fstream>  // for reading files
#include <vector>   // for using vector
#include <string>   // for using strings

using namespace std; // so we do not need std::

class Email {
public:
    string sender;  // stores sender type
    string subject; // stores email subject
    string date;    // stores email date

    Email() {} // default constructor

    Email(string s, string sub, string d) {
        sender = s;      // save sender
        subject = sub;   // save subject
        date = d;        // save date
    }
};

class MaxHeap {
private:
    vector<Email> heap; // stores the emails

    int getPriority(string sender) {
        if (sender == "Boss") // highest priority
            return 5;
        else if (sender == "Subordinate") // second priority
            return 4;
        else if (sender == "Peer") // third priority
            return 3;
        else if (sender == "ImportantPerson") // fourth priority
            return 2;
        else // lowest priority
            return 1;
    }

    int convertDate(string date) {
        string month = date.substr(0, 2); // get month
        string day = date.substr(3, 2);   // get day
        string year = date.substr(6, 4);  // get year

        return stoi(year + month + day); // turn date into a number
    }

    bool higherPriority(Email a, Email b) {
        int p1 = getPriority(a.sender); // get first email priority
        int p2 = getPriority(b.sender); // get second email priority

        if (p1 > p2) // first email is higher
            return true;

        if (p1 < p2) // second email is higher
            return false;

        return convertDate(a.date) > convertDate(b.date); // newer one goes first
    }

    void heapifyUp(int index) {
        while (index > 0) { // keep moving up if needed
            int parent = (index - 1) / 2; // find parent

            if (higherPriority(heap[index], heap[parent])) {
                swap(heap[index], heap[parent]); // swap with parent
                index = parent; // move to parent position
            }
            else {
                break; // stop if order is right
            }
        }
    }

    void heapifyDown(int index) {
        int size = heap.size(); // get heap size

        while (true) {
            int left = 2 * index + 1;  // left child
            int right = 2 * index + 2; // right child
            int largest = index;       // start with current index

            if (left < size && higherPriority(heap[left], heap[largest])) {
                largest = left; // left has higher priority
            }

            if (right < size && higherPriority(heap[right], heap[largest])) {
                largest = right; // right has higher priority
            }

            if (largest != index) {
                swap(heap[index], heap[largest]); // swap
                index = largest; // move down
            }
            else {
                break; // stop if order is right
            }
        }
    }

public:
    void insert(Email email) {
        heap.push_back(email); // add email
        heapifyUp(heap.size() - 1); // move it to the right place
    }

    bool isEmpty() {
        return heap.empty(); // check if there are no emails
    }

    int count() {
        return heap.size(); // return number of emails
    }

    Email getMax() {
        return heap[0]; // return highest priority email
    }

    void removeMax() {
        if (heap.empty()) // stop if there are no emails
            return;

        heap[0] = heap.back(); // move last email to the top
        heap.pop_back(); // remove last email

        if (!heap.empty()) // fix heap if emails are left
            heapifyDown(0);
    }
};

int main() {
    string fileName; // stores file name

    cout << "Enter file name: "; // ask for file name
    cin >> fileName; // read file name

    ifstream inputFile(fileName); // open the file

    if (!inputFile) {
        cout << "Could not open file." << endl; // show error
        return 1; // stop program
    }

    MaxHeap emails; // create email heap
    string line; // stores each line

    while (getline(inputFile, line)) { // read file line by line

        if (line.substr(0, 5) == "EMAIL") { // check for EMAIL
            string data = line.substr(6); // remove EMAIL part

            int comma1 = data.find(','); // find first comma
            int comma2 = data.find(',', comma1 + 1); // find second comma

            string sender = data.substr(0, comma1); // get sender

            string subject =
                data.substr(comma1 + 1,
                            comma2 - comma1 - 1); // get subject

            string date =
                data.substr(comma2 + 1); // get date

            Email email(sender, subject, date); // create email object
            emails.insert(email); // add it to heap
        }

        else if (line == "NEXT") { // check for NEXT
            if (!emails.isEmpty()) { // make sure there is an email
                Email next = emails.getMax(); // get top email

                cout << "Next email:" << endl; // show heading
                cout << "Sender: " << next.sender << endl; // show sender
                cout << "Subject: " << next.subject << endl; // show subject
                cout << "Date: " << next.date << endl; // show date
            }
        }

        else if (line == "READ") { // check for READ
            if (!emails.isEmpty()) { // make sure there is an email
                emails.removeMax(); // remove top email
            }
        }

        else if (line == "COUNT") { // check for COUNT
            cout << "There are "
                 << emails.count()
                 << " emails to read."
                 << endl; // show unread count
        }
    }

    inputFile.close(); // close file

    return 0; // end program
}