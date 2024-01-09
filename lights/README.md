# Lights example plugin

This is an example plugin that demonstrates how to write a plugin that can manage vehicle specific lights panels. It
will receive and respond to stanag 1302 messages governing its configuration, as well as using STANAG message 44 to
update the commanded power state of individual lights. It also will accept reports of lights status from STANAG message
107\.

Overall it is a workable example of a full stack feature that includes STANAG messaging, UI integration, vehicle
specificity, and use of the core APIs provided by VCSi.
