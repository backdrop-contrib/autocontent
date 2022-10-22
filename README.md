Automatic Content
==================

Autocontent allows content to be created automatically, at defined intervals. A
new froum post can be creaed weekly, or recurring events created daily.

This module is most useful when the content is identical, or can be computed 
using tokens.


Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://docs.backdropcms.org/documentation/extend-with-modules.

- Visit the configuration page under Administration > Configuration > Content >
  Automatic Content (admin/config/content/autocontent) and define the types of content
  to be created, along with pre-defined values.

- Visit the cron task page under Administration > Configuration > System > Cron
  (admin/config/system/cron) to confirm that the cron task happens at least as 
  often as the interval defined.


Issues
------

Bugs and Feature Requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/autocontent/issues.


Current Maintainers
-------------------

- [JenLampton](https://github.com/jenlampton)
- Seeking additional maintainers


Credits
-------

- Created for Backdrop CMS by [JenLampton](https://github.com/jenlampton).


License
-------

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.
