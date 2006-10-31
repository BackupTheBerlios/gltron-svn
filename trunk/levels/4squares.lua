directions = { random = -1, up = 2, down = 0, right = 1, left = 3 }

level = {

version = 71,

-- the sizes in the level are all in the [0,1] range and can be
-- scaled to the appropriate arena size

scalable = 1,

-- collision detection takes place against these lines
boundary = {
  { { x = 0, y = 1 }, { x = 0.375, y = 1 } },
  { { x = 0, y = 1 }, { x = 0, y = 0.625 } },
  { { x = 0, y = 0 }, { x = 0, y = 0.375 } },
  { { x = 0, y = 0 }, { x = 0.375, y = 0 } },
  { { x = 0, y = 0.375 }, { x = 0.125, y = 0.375 } },
  { { x = 0.125, y = 0.375 }, { x = 0.125, y = 0.625 } },
  { { x = 0.125, y = 0.625 }, { x = 0, y = 0.625 } },
  { { x = 0.125, y = 0.375 }, { x = 0.125, y = 0.625 } },
  { { x = 0.125, y = 0.375 }, { x = 0.125, y = 0.625 } },
  { { x = 0, y = 0 }, { x = 0, y = 0.375 } },
  { { x = 0.375, y = 0 }, { x = 0.375, y = 0.125 } },
  { { x = 0.375, y = 0.125 }, { x = 0.625, y = 0.125 } },
  { { x = 0.625, y = 0.125 }, { x = 0.625, y = 0 } },
  { { x = 0.625, y = 0 }, { x = 1, y = 0 } },
  { { x = 1, y = 0 }, { x = 1, y = 0.375 } },
  { { x = 1, y = 0.375 }, { x = 0.875, y = 0.375 } },
  { { x = 0.875, y = 0.625 }, { x = 0.875, y = 0.375 } },
  { { x = 0.875, y = 0.625 }, { x = 1, y = 0.625 } },
  { { x = 1, y = 1 }, { x = 1, y = 0.625 } },
  { { x = 0.625, y = 1 }, { x = 1, y = 1 } },
  { { x = 0.625, y = 1 }, { x = 0.625, y = 0.875 } },
  { { x = 0.375, y = 0.875 }, { x = 0.625, y = 0.875 } },
  { { x = 0.375, y = 0.875 }, { x = 0.375, y = 1 } },
  { { x = 0.25, y = 0.375 }, { x = 0.375, y = 0.375 } },
  { { x = 0.375, y = 0.375 }, { x = 0.375, y = 0.25 } },
  { { x = 0.375, y = 0.25 }, { x = 0.625, y = 0.25 } },
  { { x = 0.625, y = 0.25 }, { x = 0.625, y = 0.375 } },
  { { x = 0.625, y = 0.375 }, { x = 0.75, y = 0.375 } },
  { { x = 0.75, y = 0.375 }, { x = 0.75, y = 0.625 } },
  { { x = 0.75, y = 0.625 }, { x = 0.625, y = 0.625 } },
  { { x = 0.625, y = 0.625 }, { x = 0.625, y = 0.75 } },
  { { x = 0.625, y = 0.75 }, { x = 0.375, y = 0.75 } },
  { { x = 0.375, y = 0.75 }, { x = 0.375, y = 0.625 } },
  { { x = 0.375, y = 0.625 }, { x = 0.25, y = 0.625 } },
  { { x = 0.25, y = 0.375 }, { x = 0.25, y = 0.625 } },
},

-- spawn points
-- (they don't have to be sorted, they will be randomized anyway)

spawn = {
  { x = .25, y = .5, dir = directions.random }, 
  { x = .5, y = .25, dir = directions.random }, 
  { x = .5, y = .75, dir = directions.random }, 
  { x = .75, y = .5, dir = directions.random }
},

  -- floor geometry is used to generate reflections, and
  -- as a background for the 2d map
  floor = {
    -- uv = 4, normal = 2, position = 1
    vertexformat = 5, 
     
    shading = {
      lit = 0, -- no lighting, only diffuse texture is applied
      textures = { 
         diffuse = {
            file = "gltron_floor.png",
            -- 0: nearest, 1: linear: 2: linear_mipmap_nearest
            -- 3: linear_mipmap_linear
            min_filter = 3,
            mag_filter = 1,
            -- 0: clamp, 1: clamp to edge, 2: repeat
            wrap_s = 2,
            wrap_t = 2
         }
      }
    },
    vertices = {
      { pos = { x = 0, y = 0, z = 0 }, uv = { u = 0, v = 0 } },
      { pos = { x = 1, y = 0, z = 0 }, uv = { u = 40, v = 0 } },
      { pos = { x = 1, y = 1, z = 0 }, uv = { u = 40, v = 40 } },
      { pos = { x = 0, y = 1, z = 0 }, uv = { u = 0, v = 40 } }
    },
    indices = { 
      { 0, 1, 2 },
      { 0, 2, 3 }
    }
  },

  arena = {
    -- uv = 4, normal = 2, position = 1
    vertexformat = 5, 
    shading = {
      lit = 0, -- no lighting, only diffuse texture is applied
      textures = { 
         diffuse = {
            file = "walls.png",
            -- 0: nearest, 1: linear: 2: linear_mipmap_nearest
            -- 3: linear_mipmap_linear
            min_filter = 3,
            mag_filter = 1,
            -- 0: clamp, 1: clamp to edge, 2: repeat
            wrap_s = 1,
            wrap_t = 1
         }
      }
    },
    vertices = {
      -- wall
      { pos = { x = 0, y = 0, z = 0 },        uv = { u = 0, v = 0 } },
      { pos = { x = 0, y = 1, z = 0 },        uv = { u = 0.25, v = 0 } },
      { pos = { x = 0, y = 1, z = 0.125 },    uv = { u = 0.25, v = 1 } },
      { pos = { x = 0, y = 0, z = 0.125 },    uv = { u = 0, v = 1 } },
      -- wall
      { pos = { x = 0, y = 1, z = 0 },        uv = { u = 0.25, v = 0 } },
      { pos = { x = 1, y = 1, z = 0 },        uv = { u = 0.5, v = 0 } },
      { pos = { x = 1, y = 1, z = 0.125 },    uv = { u = 0.5, v = 1 } },
      { pos = { x = 0, y = 1, z = 0.125 },    uv = { u = 0.25, v = 1 } },
      -- wall
      { pos = { x = 1, y = 1, z = 0 },        uv = { u = 0.5, v = 0 } },
      { pos = { x = 1, y = 0, z = 0 },        uv = { u = 0.75, v = 0 } },
      { pos = { x = 1, y = 0, z = 0.125 },    uv = { u = 0.75, v = 1 } },
      { pos = { x = 1, y = 1, z = 0.125 },    uv = { u = 0.5, v = 1 } },
      -- wall
      { pos = { x = 1, y = 0, z = 0 },        uv = { u = 0.75, v = 0 } },
      { pos = { x = 0, y = 0, z = 0 },        uv = { u = 1, v = 0 } },
      { pos = { x = 0, y = 0, z = 0.125 },    uv = { u = 1, v = 1 } },
      { pos = { x = 1, y = 0, z = 0.125 },    uv = { u = 0.75, v = 1 } }
    },
    indices = { 
      { 0, 1, 2 }, { 0, 2, 3 },
      { 4, 5, 6 }, { 4, 6, 7 }, { 8, 9, 10 }, { 8, 10, 11 },
      { 12, 13, 14 }, { 12, 14, 15 }
    }
  }

}
