
## Mixer Unreal Blank Instructions

Mixer Unreal Blank is a sample project intended to provide functional reference code for how to do basic things from unreal like send messages and update participants.

These are instructions for running the Mixer Unreal Blank unreal project using Unreal Editor 4.19.2

If you want, you can just download the project and use the code as reference.


## How to Run the Mixer Unreal Blank Project


### Setup

Make sure you have Unreal and Visual Studio installed.  See instructions below, if you need them.

Install the mixer plugins as described here: https://github.com/mixer/interactive-unreal-plugin/wiki/Getting-started-using-Blueprints

Download this project from github


### Running the Unreal Game Client

Open the unreal project in the unreal editor.

Run.  You should be asked to sign in to mixer.  [Lyle: verify.  If not, we'll need to move the signin instructions up here]

If you go to Mixer.com/[yourchannel], you should see interactive controls (a button and a red box).


### Changing the Custom Controls

By default, this unreal project will use the currently uploaded bundle for the "mixblank" interactive project.  If you want to make changes to the custom controls, you'll need to use your own interactive project.  You can use the participant_experience code as a starting point if you like, and change the name of the project for uploading.  Please refer to the instructions below ("Specifying an Interactive Project") for how to use a different interactive project in Unreal.


### Experimenting, making changes…

What follows are steps for building something like mixblank from scratch.  If you want to experiment with mixblank, you can use whatever instructions below are helpful.


## 

---



## Setting Up An Unreal Mixer Project From Scratch


### Setting Up Unreal

Install Unreal and Visual Studio if not already installed.  See "Installing Unreal and Visual Studio" Below.

Install the mixer plugins as described here: https://github.com/mixer/interactive-unreal-plugin/wiki/Getting-started-using-Blueprints

Create a new project in Unreal, using the setting you wish. In our case, we created a blank, Blueprint project.

Open your new project and to go to Settings -> Plugins.

Check "enabled" for the mixer plugin. Unreal will require a restart.

After restarting, set up your mixer plugin settings as described next.


#### Specifying an Interactive Project

go to Settings -> Project settings and scroll down the menu on the left until you find the Plugins section. Click on "Mixer Interactivity."

Under Auth, click on the link for "OAuth Clients Page" and follow the directions there to generate a Client Id or use an existing one.

Copy the client Id you are using and paste it into the Client Id field.

Set the Redirect Uri to "*.mixer.com"

Click on the "Login" button to the far right of the Game Binding section and sign in to your Mixer account. If the button is disabled, ensure that the "From Mixer User" radio button is active.

Approve permissions for the OAth client.

Once you are signed in you can select your interactive project from Game Name pulldown.

Alternatively, you can use another an interactive project you don't own so long as you have permission to do so and know the projects game version id.  To use an interactive project this way, select the "Manual" radio button a the top of the Game Binding section and then enter the Game Version Id into the Game Version Id field.

Once you have a mixer client running in your project (see below), you should be able to connect to Mixer and see any custom controls you've set up (if you've uploaded a bundle).

Create a new blueprint with the parent class of of your game client class (e.g. MixerGameClient). In the MixBlank example this is the BP_MixerGameClient blueprint.

Place the blueprint somewhere it will be loaded with the game. In our example we opted to add it to our gamemode (MixBlankGameMode) as a child actor. This actor can be anywhere as long as "Event BeginPlay" will be triggered. BeginPlay is where the client will subscribe to events.

Many of the functions and events in mixer client are static and can be accessed from any blueprint. The remaining events and functions must be accessed through the blueprint actor you've set up for your mixer game client. Delegates such as OnParticipantEvent must be bound to events in the game client blueprint.


### Creating An Interactive Project

For our purposes, the control schema can be as simple as this, and you don't need to position controls visually, just define them in the default scene.

    {

    scenes: [{

      controls: [

        {

          controlID: "anchor",

          kind: "button",

          position: [],

          text: "test control for messages and updates"

        }

      ],

      sceneID: "default"

    }]

    }


#### Simple Walkthrough



*   Go to the mixer developer lab and create a new mixplay project.
*   Go to the "Build" tab.
*   Create a button control and name it "anchor."  There is no need to add the button to the grid.
*   Save the interactive project.

That's it.  You might want to write down the app version id from the Code tab.


### Adding a C++ Module to the Unreal Project

Follow these instructions:

[https://github.com/mixer/interactive-unreal-plugin/wiki/Getting-started-using-C-Plus-Plus](https://github.com/mixer/interactive-unreal-plugin/wiki/Getting-started-using-C-Plus-Plus#build-the-plugin)

