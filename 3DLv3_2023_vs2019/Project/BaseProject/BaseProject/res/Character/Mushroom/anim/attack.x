xof 0303txt 0032

template XSkinMeshHeader {
  <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
  WORD nMaxSkinWeightsPerVertex;
  WORD nMaxSkinWeightsPerFace;
  WORD nBones;
}

template SkinWeights {
  <6f0d123b-bad2-4167-a0d0-80224f25fabb>
  STRING transformNodeName;
  DWORD nWeights;
  array DWORD vertexIndices[nWeights];
  array float weights[nWeights];
  Matrix4x4 matrixOffset;
}

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000,-1.000000, 0.000000,
     0.000000, 1.000000,-0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame Armature {
    FrameTransformMatrix {
       0.010000, 0.000000, 0.000000, 0.000000,
       0.000000, 0.010000, 0.000000, 0.000000,
       0.000000, 0.000000, 0.010000, 0.000000,
       0.000000, 0.000000, 0.000000, 1.000000;;
    }
    Frame Armature_mushroom_root {
      FrameTransformMatrix {
        -0.000000, 0.964534, 0.263957, 0.000000,
        -0.000000,-0.263957, 0.964535, 0.000000,
         1.000000, 0.000000,-0.000000, 0.000000,
        -0.000000, 0.000000,-0.000000, 1.000000;;
      }
      Frame Armature_mushroom_spine01 {
        FrameTransformMatrix {
           0.993708,-0.112002, 0.000000, 0.000000,
           0.112001, 0.993708, 0.000000, 0.000000,
          -0.000000,-0.000000, 1.000000, 0.000000,
           0.000000,16.915751, 0.000000, 1.000000;;
        }
        Frame Armature_mushroom_spine02 {
          FrameTransformMatrix {
             0.872184,-0.489177, 0.000000, 0.000000,
             0.489177, 0.872184, 0.000000, 0.000000,
            -0.000000, 0.000000, 1.000000, 0.000000,
             0.992054,18.436659, 0.000000, 1.000000;;
          }
          Frame Armature_mushroom_leftEye {
            FrameTransformMatrix {
               0.974144, 0.225926, 0.000000, 0.000000,
              -0.000000, 0.000000, 1.000000, 0.000000,
               0.225926,-0.974144, 0.000000, 0.000000,
              -12.626502, 3.605484,10.760423, 1.000000;;
            }
          } // End of Armature_mushroom_leftEye
          Frame Armature_mushroom_mouthAngry {
            FrameTransformMatrix {
               0.974144, 0.225926,-0.000000, 0.000000,
              -0.225926, 0.974144, 0.000000, 0.000000,
               0.000000, 0.000000, 1.000000, 0.000000,
              -11.038952,-6.100142, 0.000000, 1.000000;;
            }
          } // End of Armature_mushroom_mouthAngry
          Frame Armature_mushroom_mouthSmile {
            FrameTransformMatrix {
               0.974144, 0.225926,-0.000000, 0.000000,
              -0.225926, 0.974144, 0.000000, 0.000000,
               0.000000, 0.000000, 1.000000, 0.000000,
               5.163269,-2.228558,-0.000000, 1.000000;;
            }
          } // End of Armature_mushroom_mouthSmile
          Frame Armature_mushroom_rightEye {
            FrameTransformMatrix {
              -0.974144,-0.225926, 0.000000, 0.000000,
               0.000000, 0.000000, 1.000000, 0.000000,
              -0.225926, 0.974144,-0.000000, 0.000000,
              -12.626534, 3.605513,-10.760398, 1.000000;;
            }
          } // End of Armature_mushroom_rightEye
          Frame Armature_mushroom_spine03 {
            FrameTransformMatrix {
               0.998666, 0.051632,-0.000000, 0.000000,
              -0.051632, 0.998666, 0.000000, 0.000000,
               0.000000, 0.000000, 1.000000, 0.000000,
               0.000001,19.089918, 0.000000, 1.000000;;
            }
            Frame Armature_mushroom_head {
              FrameTransformMatrix {
                 0.000000,-1.000000, 0.000000, 0.000000,
                -1.000000,-0.000000,-0.000000, 0.000000,
                 0.000000,-0.000000,-1.000000, 0.000000,
                 0.000003,26.191212, 0.000000, 1.000000;;
              }
              Frame Armature_mushroom_skirt01 {
                FrameTransformMatrix {
                   0.834784,-0.550578, 0.000000, 0.000000,
                   0.550578, 0.834784, 0.000000, 0.000000,
                  -0.000000,-0.000000, 1.000000, 0.000000,
                   5.301030,22.058914, 0.000001, 1.000000;;
                }
                Frame Armature_mushroom_skirt02 {
                  FrameTransformMatrix {
                     0.985405,-0.170225,-0.000000, 0.000000,
                     0.170225, 0.985405, 0.000000, 0.000000,
                     0.000000,-0.000000, 1.000000, 0.000000,
                     0.000011,28.317768, 0.000000, 1.000000;;
                  }
                } // End of Armature_mushroom_skirt02
              } // End of Armature_mushroom_skirt01
              Frame Armature_mushroom_skirt03 {
                FrameTransformMatrix {
                  -0.838952,-0.544208,-0.000000, 0.000000,
                   0.544208,-0.838950, 0.000000, 0.000000,
                  -0.000000, 0.000000, 1.000000, 0.000000,
                   7.062398,-28.739540, 0.000001, 1.000000;;
                }
                Frame Armature_mushroom_skirt04 {
                  FrameTransformMatrix {
                     0.972785, 0.231709,-0.000000, 0.000000,
                    -0.231709, 0.972785, 0.000000, 0.000000,
                     0.000000,-0.000000, 1.000000, 0.000000,
                     0.000002,27.726759,-0.000000, 1.000000;;
                  }
                } // End of Armature_mushroom_skirt04
              } // End of Armature_mushroom_skirt03
              Frame Armature_mushroom_skirt05 {
                FrameTransformMatrix {
                   0.000020,-1.000000, 0.000000, 0.000000,
                   0.569210, 0.000011,-0.822192, 0.000000,
                   0.822192, 0.000017, 0.569210, 0.000000,
                   4.670747,-0.000431,-27.098680, 1.000000;;
                }
                Frame Armature_mushroom_skirt06 {
                  FrameTransformMatrix {
                     1.000000, 0.000000, 0.000000, 0.000000,
                    -0.000000, 0.980316, 0.197437, 0.000000,
                    -0.000000,-0.197437, 0.980316, 0.000000,
                     0.000000,30.082026, 0.000005, 1.000000;;
                  }
                } // End of Armature_mushroom_skirt06
              } // End of Armature_mushroom_skirt05
              Frame Armature_mushroom_skirt07 {
                FrameTransformMatrix {
                   0.000020,-1.000000, 0.000000, 0.000000,
                   0.569208, 0.000012, 0.822194, 0.000000,
                  -0.822194,-0.000017, 0.569208, 0.000000,
                   4.670796,-0.000435,27.098694, 1.000000;;
                }
                Frame Armature_mushroom_skirt08 {
                  FrameTransformMatrix {
                     1.000000, 0.000000,-0.000000, 0.000000,
                    -0.000000, 0.980269,-0.197665, 0.000000,
                     0.000000, 0.197665, 0.980270, 0.000000,
                    -0.000002,30.081970, 0.000011, 1.000000;;
                  }
                } // End of Armature_mushroom_skirt08
              } // End of Armature_mushroom_skirt07
            } // End of Armature_mushroom_head
          } // End of Armature_mushroom_spine03
        } // End of Armature_mushroom_spine02
      } // End of Armature_mushroom_spine01
    } // End of Armature_mushroom_root
    Frame mushroom {
      FrameTransformMatrix {
         1.000000,-0.000000, 0.000000, 0.000000,
        -0.000000, 1.000000, 0.000000, 0.000000,
         0.000000, 0.000000, 1.000000, 0.000000,
         0.000004,-0.000000, 0.000001, 1.000000;;
      }
    } // End of mushroom
  } // End of Armature
} // End of Root
