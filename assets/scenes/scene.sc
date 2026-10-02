{
  "name": "ProceduralTestScene",
  "objects": [
    {
      "name": "MainPlayer",
      "type": "Player",
      "position": {
        "x": 0,
        "y": 2,
        "z": 0
      },
      "components": [
        {
          "type": "CameraComponent"
        },
        {
          "type": "PlayerControllerComponent"
        },
        {
          "type": "AudioListenerComponent"
        },
        {
          "type": "AudioComponent",
          "audio": [
            {
              "name": "shoot",
              "path": "audio/shoot.wav"
            },
            {
              "name": "step",
              "path": "audio/step.wav"
            },
            {
              "name": "jump",
              "path": "audio/jump.wav"
            }
          ]
        }
      ],
      "children": [
        {
          "name": "Gun",
          "type": "gltf",
          "path": "models/sten_gunmachine_carbine/scene.gltf",
          "position": {
            "x": 0.75,
            "y": -0.5,
            "z": -0.75
          },
          "scale": {
            "x": -1.0,
            "y": 1.0,
            "z": 1.0
          }
        }
      ]
    },
    {
      "name": "SpawnGround",
      "position": {
        "x": 0,
        "y": 0,
        "z": 0
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 10,
            "y": 1,
            "z": 10
          },
          "material": {
            "path": "materials/checker.mat"
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 10,
            "y": 1,
            "z": 10
          },
          "body": {
            "mass": 0,
            "friction": 0.5,
            "type": "static"
          }
        }
      ]
    },
    {
      "name": "Template_Wall",
      "position": {
        "x": 5,
        "y": -10,
        "z": 5
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 2,
            "y": 2,
            "z": 2
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 0.8,
                  "value1": 0.2,
                  "value2": 0.2
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 2,
            "y": 2,
            "z": 2
          },
          "body": {
            "mass": 0,
            "friction": 0.5,
            "type": "static"
          }
        }
      ]
    },
    {
      "name": "Template_Floor",
      "position": {
        "x": 10,
        "y": -10,
        "z": 5
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 2,
            "y": 0.2,
            "z": 2
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 0.3,
                  "value1": 0.3,
                  "value2": 0.8
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 2,
            "y": 0.2,
            "z": 2
          },
          "body": {
            "mass": 0,
            "friction": 0.5,
            "type": "static"
          }
        }
      ]
    },
    {
      "name": "Light",
      "position": {
        "x": -2,
        "y": 15,
        "z": 2
      },
      "components": [
        {
          "type": "LightComponent",
          "color": {
            "r": 1,
            "g": 1,
            "b": 1
          }
        }
      ]
    }
  ],
  "camera": "MainPlayer"
}