{
  "name": "UmbraCore_Playground",
  "objects": [
    {
      "name": "MainPlayer",
      "type": "Player",
      "position": {
        "x": 0,
        "y": 2,
        "z": -7
      },
      "components": [
        { "type": "CameraComponent" },
        { "type": "PlayerControllerComponent" },
        { "type": "AudioListenerComponent" },
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
      "name": "Ground",
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
            "x": 30,
            "y": 1,
            "z": 30
          },
          "material": { "path": "materials/floor.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 30,
            "y": 1,
            "z": 30
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
      "name": "Wall_Left",
      "position": {
        "x": -15.5,
        "y": 3,
        "z": 0
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1,
            "y": 5,
            "z": 30
          },
          "material": { "path": "materials/wall.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1,
            "y": 5,
            "z": 30
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
      "name": "Wall_Right",
      "position": {
        "x": 15.5,
        "y": 3,
        "z": 0
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1,
            "y": 5,
            "z": 30
          },
          "material": { "path": "materials/wall.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1,
            "y": 5,
            "z": 30
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
      "name": "Wall_Front",
      "position": {
        "x": 0,
        "y": 3,
        "z": -15.5
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 30,
            "y": 5,
            "z": 1
          },
          "material": { "path": "materials/wall.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 30,
            "y": 5,
            "z": 1
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
      "name": "Wall_Back",
      "position": {
        "x": 0,
        "y": 3,
        "z": 15.5
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 30,
            "y": 5,
            "z": 1
          },
          "material": { "path": "materials/wall.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 30,
            "y": 5,
            "z": 1
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
      "name": "Platform_Corner",
      "position": {
        "x": -14,
        "y": 1.05,
        "z": -14
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 2,
            "y": 1.1,
            "z": 2
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 0.3,
                  "value1": 0.7,
                  "value2": 0.9
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
            "y": 1.1,
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
      "name": "Platform_Long",
      "position": {
        "x": -14,
        "y": 1.05,
        "z": -3
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 2,
            "y": 1.1,
            "z": 8
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 0.3,
                  "value1": 0.7,
                  "value2": 0.9
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
            "y": 1.1,
            "z": 8
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
      "name": "Platform_Bridge",
      "position": {
        "x": -7,
        "y": 1.05,
        "z": -3
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 8,
            "y": 1.1,
            "z": 2
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 0.3,
                  "value1": 0.7,
                  "value2": 0.9
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 8,
            "y": 1.1,
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
      "name": "JumpPad",
      "position": {
        "x": -7,
        "y": 1.75,
        "z": 1
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
                  "value0": 1.0,
                  "value1": 0.1,
                  "value2": 0.1
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
      "name": "Stair_1",
      "position": {
        "x": -12.5,
        "y": 1.25,
        "z": -14
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 0.5,
            "y": 0.2,
            "z": 2
          },
          "material": { "path": "materials/checker.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 0.5,
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
      "name": "Stair_2",
      "position": {
        "x": -11.5,
        "y": 0.95,
        "z": -14
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 0.5,
            "y": 0.2,
            "z": 2
          },
          "material": { "path": "materials/checker.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 0.5,
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
      "name": "Stair_3",
      "position": {
        "x": -10.5,
        "y": 0.65,
        "z": -14
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 0.5,
            "y": 0.2,
            "z": 2
          },
          "material": { "path": "materials/checker.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 0.5,
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
      "name": "InternalWall_Cover",
      "position": {
        "x": -1,
        "y": 1.5,
        "z": -11
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1,
            "y": 2,
            "z": 4
          },
          "material": { "path": "materials/wall.mat" }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1,
            "y": 2,
            "z": 4
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
      "name": "PhysicsTower_Base1",
      "position": {
        "x": 8,
        "y": 1.25,
        "z": -2
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 1.0,
                  "value1": 0.0,
                  "value2": 0.0
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "body": {
            "mass": 15,
            "friction": 0.5,
            "type": "dynamic"
          }
        }
      ]
    },
    {
      "name": "PhysicsTower_Base2",
      "position": {
        "x": 8,
        "y": 1.25,
        "z": 0
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 0.0,
                  "value1": 1.0,
                  "value2": 0.0
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "body": {
            "mass": 15,
            "friction": 0.5,
            "type": "dynamic"
          }
        }
      ]
    },
    {
      "name": "PhysicsTower_Base3",
      "position": {
        "x": 8,
        "y": 1.25,
        "z": 2
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 0.0,
                  "value1": 0.0,
                  "value2": 1.0
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "body": {
            "mass": 15,
            "friction": 0.5,
            "type": "dynamic"
          }
        }
      ]
    },
    {
      "name": "PhysicsTower_Mid1",
      "position": {
        "x": 8,
        "y": 2.76,
        "z": -1
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 1.0,
                  "value1": 1.0,
                  "value2": 0.0
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "body": {
            "mass": 15,
            "friction": 0.5,
            "type": "dynamic"
          }
        }
      ]
    },
    {
      "name": "PhysicsTower_Mid2",
      "position": {
        "x": 8,
        "y": 2.76,
        "z": 1
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 0.0,
                  "value1": 1.0,
                  "value2": 1.0
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "body": {
            "mass": 15,
            "friction": 0.5,
            "type": "dynamic"
          }
        }
      ]
    },
    {
      "name": "PhysicsTower_Top",
      "position": {
        "x": 8,
        "y": 4.27,
        "z": 0
      },
      "components": [
        {
          "type": "MeshComponent",
          "mesh": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "material": {
            "path": "materials/checker.mat",
            "params": {
              "float3": [
                {
                  "name": "color",
                  "value0": 1.0,
                  "value1": 0.0,
                  "value2": 1.0
                }
              ]
            }
          }
        },
        {
          "type": "PhysicsComponent",
          "collider": {
            "type": "box",
            "x": 1.5,
            "y": 1.5,
            "z": 1.5
          },
          "body": {
            "mass": 15,
            "friction": 0.5,
            "type": "dynamic"
          }
        }
      ]
    },
    {
      "name": "Light_Main",
      "position": {
        "x": 0,
        "y": 15,
        "z": 0
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