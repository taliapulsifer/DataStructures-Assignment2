//Talia Pulsifer
// 09/09/23
// FILE: IntSet.cpp - header file for IntSet class
//       Implementation file for the IntStore class
//       (See IntSet.h for documentation.)
// INVARIANT for the IntSet class:
// (1) Distinct int values of the IntSet are stored in a 1-D,
//     dynamic array whose size is stored in member variable
//     capacity; the member variable data references the array.
// 
// (2) The distinct int value with earliest membership is stored
//     in data[0], the distinct int value with the 2nd-earliest
//     membership is stored in data[1], and so on.
//     Note: No "prior membership" information is tracked; i.e.,
//           if an int value that was previously a member (but its
//           earlier membership ended due to removal) becomes a
//           member again, the timing of its membership (relative
//           to other existing members) is the same as if that int
//           value was never a member before.
//     Note: Re-introduction of an int value that is already an
//           existing member (such as through the add operation)
//           has no effect on the "membership timing" of that int
//           value.
// (4) The # of distinct int values the IntSet currently contains
//     is stored in the member variable used.
// 
// (5) Except when the IntSet is empty (used == 0), ALL elements
//     of data from data[0] until data[used - 1] contain relevant
//     distinct int values; i.e., all relevant distinct int values
//     appear together (no "holes" among them) starting from the
//     beginning of the data array.
// 
// (6) We DON'T care what is stored in any of the array elements
//     from data[used] through data[capacity - 1].
//     Note: This applies also when the IntSet is empry (used == 0)
//           in which case we DON'T care what is stored in any of
//           the data array elements.
// 
//     Note: A distinct int value in the IntSet can be any of the
//           values an int can represent (from the most negative
//           through 0 to the most positive), so there is no
//           particular int value that can be used to indicate an
//           irrelevant value. But there's no need for such an
//           "indicator value" since all relevant distinct int
//           values appear together starting from the beginning of
//           the data array and used (if properly initialized and
//           maintained) should tell which elements of the data
//           array are actually relevant.
//
// DOCUMENTATION for private member (helper) function:
//   void resize(int new_capacity)
//     Pre:  (none)
//           Note: Recall that one of the things a constructor
//                 has to do is to make sure that the object
//                 created BEGINS to be consistent with the
//                 class invariant. Thus, resize() should not
//                 be used within constructors unless it is at
//                 a point where the class invariant has already
//                 been made to hold true.
//     Post: The capacity (size of the dynamic array) of the
//           invoking IntSet is changed to new_capacity...
//           ...EXCEPT when new_capacity would not allow the
//           invoking IntSet to preserve current contents (i.e.,
//           value for new_capacity is invalid or too low for the
//           IntSet to represent the existing collection),...
//           ...IN WHICH CASE the capacity of the invoking IntSet
//           is set to "the minimum that is needed" (which is the
//           same as "exactly what is needed") to preserve current
//           contents...
//           ...BUT if "exactly what is needed" is 0 (i.e. existing
//           collection is empty) then the capacity should be
//           further adjusted to 1 or DEFAULT_CAPACITY (since we
//           don't want to request dynamic arrays of size 0).
//           The collection represented by the invoking IntSet
//           remains unchanged.
//           If reallocation of dynamic array is unsuccessful, an
//           error message to the effect is displayed and the
//           program unconditionally terminated.

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;


//Constructor
IntSet::IntSet(int initial_capacity)
    : used(0)
    , capacity(initial_capacity)
{
    data = new int [capacity];
}

//Copy Constructor
IntSet::IntSet(const IntSet& src)
{
    this->capacity = src.capacity;
    this->used = src.used;
    this->data = new int[capacity];
    memcpy_s(data, capacity, src.data, src.used);
}

//Desructor, loop through elements and delete them
IntSet::~IntSet()
{
    delete(data);
}

IntSet& IntSet::operator=(const IntSet& rhs)
{
    this->capacity = rhs.capacity;
    this->used = rhs.used;
    delete(this->data);
    this->data = new int[rhs.capacity];
    memcpy_s(data, capacity, rhs.data, rhs.used);
    return *this;
}

int IntSet::size() const
{
    return used;
}

//
void IntSet::resize(int new_capacity)
{
    //Dynamic storage array
    int* temp = new int[new_capacity];
    int newUsed = min(used, new_capacity);// if new capacity is less than used, make used equal new capacity

    //copy them into a larger array
    memcpy_s(temp, new_capacity, data, newUsed);
    
    //Delete the original array
    delete(this->data);

    //Put the new array in the original array's spot
    this->data = temp;

    //ReAssign the capacity to be new capacity
    capacity = new_capacity;
    used = newUsed;
}

bool IntSet::isEmpty() const
{
   return used == 0; 
}

bool IntSet::contains(int anInt) const
{
    for (int i = 0; i < used; i++) 
    {
        if (data[i] == anInt)
        {
            return true;
        }
    }
    return false;
}

bool IntSet::isSubsetOf(const IntSet& otherIntSet) const
{
    int originalElement;
   //Check that the subset is smaller or the same size as the other
    if (size() > otherIntSet.size())
    {
        return false;
    }
    
    //Empty sets are always subsets
    if (isEmpty() == true)
    {
        return true;
    }
    //Loop through the IntSet
    for (int i = 0; i < size(); i++)
    {
        //Container Element
        originalElement = data[i];

        //If the parent set does not contain the same element
        if (!otherIntSet.contains(originalElement))
        {
            return false;
        }
    }
    //If it exits both loops, it has to be true
    return true; 
}

void IntSet::DumpData(ostream& out) const
{  // already implemented ... DON'T change anything
   if (used > 0)
   {
      out << data[0];
      for (int i = 1; i < used; ++i)
         out << "  " << data[i];
   }
}

//The combonation of both sets
IntSet IntSet::unionWith(const IntSet& otherIntSet) const
{
   IntSet unionS(*this);
   for (int i = 0; i < otherIntSet.used; i++)
   {
       if (!unionS.contains(otherIntSet.data[i]))
       {
           unionS.add(otherIntSet.data[i]);
       }

   }
   return unionS;
}

IntSet IntSet::intersect(const IntSet& otherIntSet) const
{
   IntSet intersection;
   for (int i = 0; i < used; i++)
   {
       int originalElement = data[i];

       if (otherIntSet.contains(originalElement))
       {
           intersection.add(originalElement);
       }
   }
   return intersection;
}

IntSet IntSet::subtract(const IntSet& otherIntSet) const
{
    IntSet subtracted;
    for (int i = 0; i < used; i++)
    {
        int originalElement = data[i];
        if (!otherIntSet.contains(originalElement))
        {
            subtracted.add(originalElement);
        }
    }
   
   return subtracted;
}

void IntSet::reset()
{
    used = 0;
}

bool IntSet::add(int anInt)
{
    if (contains(anInt))
    {
        return false;
    }

    //If it contains Int then return false
    if (used == capacity)
    {
        resize(2 * capacity);
    }

    //If the array does NOT already contain the integer and 
        //the size < MAX then add:
    data[used] = anInt;
    //Make sure to increment used!!! MAINTAIN
    used++;
    return true;
}

bool IntSet::remove(int anInt)
{
    if(used > 0)
    {
        //create pointers to the start and end of array
        int* start = data;
        int* end = data + used;
        //Loop over the array elements
        //auto = int*
        for (int* pos = start; pos != end; ++pos)
        {
            if (*pos == anInt)
            {
                //Find the next space so you can shift elements downwards
                int* next = pos + 1;
                //Continue to loop over the elements in the array
                //from current pos and copy to next pos (shift the elements 
                //over one
                    //space to 'delete' the target element
                for (; pos != end; ++pos)
                {
                    //Shift the elements downwards
                    *pos = *next++;
                }
                //Exit
                used--;
                return true;
            }
        }
    }
    return false;
}

bool operator==(const IntSet& is1, const IntSet& is2)
{
    if (is1.size() != is2.size())
        return false;

    if (is1.intersect(is2).size() == is1.size())
    {
        return true;
    }

   return false;
}
