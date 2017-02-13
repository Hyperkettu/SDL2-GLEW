//
//  Vector.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/29/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Vector_h
#define Vector_h

#include "glm/glm.hpp"

namespace Fox {
    
    /**
     * Calculates cartesian vector from given vector, thus divides with w
     *
     * @param vector operated
     * @return cartesian result vector
     */
    static glm::vec3 cartesian(const glm::vec4& vector){
        return glm::vec3(vector/vector.w);
    
    }
  
}


#endif /* Vector_h */
