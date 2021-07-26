# node-fb
 This is a dead-simple library designed to provide the fastest possible interface between the Linux Framebuffer and NodeJS.

 Just `npm i git+ssh://git@github.com:J-Cale/node-fb.git` that bad boi and you're rock an rolling.
 There's just one function. It's the `open` function. Pass the name of your favourite frame buffer, and watch that buffer come back.

 ```javascript
 const obj: {
     width: number,
     height: number,
     device: string,
     data: Buffer // <- This is the one you want.
 } = open('/dev/fb0')
 ```
