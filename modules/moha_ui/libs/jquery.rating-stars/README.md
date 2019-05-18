# jQuery RatingStars

A simple but powerful jQuery plugin for rating stars.

## Informations

you can design the entire UI yourself (CSS and Icons).

## Getting Started

see the examples in the examples-directory.

### jQuery options

```
var ratingOptions = {
    selectors: {
        starsSelector: '.rating-stars',                 // the main container
        starSelector: '.rating-star',                   // the container for a single star
        starActiveClass: 'is--active',                  // the css-class for the active stars
        starHoverClass: 'is--hover',                    // the css-class for the hover stars
        starNoHoverClass: 'is--no-hover',               // the css class for the other stars (only in the hover effect)
        targetFormElementSelector: '.rating-value'      // the selector for the input
    }
};
```

### default html template 

```
<div class="rating-stars block" id="another-rating">
    <input type="hidden" class="rating-value" name="stars-value" id="stars-value" value="2">
    <!-- set the input-value to set the default value -->
    <div class="rating-stars-container">
        <div class="rating-star">
            <i class="fa fa-star"></i>
        </div>
        <div class="rating-star">
            <i class="fa fa-star"></i>
        </div>
        <div class="rating-star">
            <i class="fa fa-star"></i>
        </div>
        <div class="rating-star">
            <i class="fa fa-star"></i>
        </div>
        <div class="rating-star">
            <i class="fa fa-star"></i>
        </div>
    </div>
</div>
```

### events

you can listen on some events to work with the plugin

#### ratingChanged
this event is triggered if the user select a star (click or touch on a star)

```
$(".rating-stars").ratingStars(ratingOptions);

$(".rating-stars").on("ratingChanged", function (ev, data) {
    // dome something
});
```

data is an object like this:

```
{
    ratingValue: 4
}
```

#### ratingOnEnter
this event is triggered if the user hovers a star

```
$(".rating-stars").ratingStars(ratingOptions);

$(".rating-stars").on("ratingOnEnter", function (ev, data) {
    // dome something
});
```

data is an object like this:

```
{
    ratingValue: 4
}
```

#### ratingOnLeave
this event is triggered if the user leaves a star

```
$(".rating-stars").ratingStars(ratingOptions);

$(".rating-stars").on("ratingOnLeave", function (ev, data) {
    // dome something
});
```

data is an object like this:

```
{
    ratingValue: 4
}
```