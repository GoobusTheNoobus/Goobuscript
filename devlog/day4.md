# Day 4

## Website
I changed a bit of formatting on the website to accomadate for smaller screens. I realized
the complexity in my formatting, and since I'm sticking to minimalism, I just centered the 
title area. It looks just as good, is a few lines of code shorter, and will take off a good
30 minutes of fixing future formatting issues. 

I also added a VERY basic footer. I didn't want to overcomplicating things, so I just wrote
the text and added some basic margin. The footer's text consists of copyright text and a notice
saying this project was created without using any LLM-generated code (though some aspects
were fact checked by AI).

## Runtime
I added booleans. And logical operators, though they worked differently than I expected. In JS,
logical operators like || and && do not return a boolean, they return one of the values that was 
compared. For example, the || logical OR operator returns the first truthy operand, or the last operand if neither is truthy. The && operator returns the first value that is 
falsy, and falls back to the last value if all are truthy. 

I had to decide between C-style logical operations or JS-style. I ultimately chose JS-style, since
my language is based off of Javascript, and it is actually quite intuitive if you really think about 
it. For example, take the expressions `str || "Fallback text"`; If `str` is falsy (empty), it 
falls back to the fallback text. It's a shorter way of writing 
`str.length > 0 ? str : "Fallback text"`.

thats basically it god bless <3