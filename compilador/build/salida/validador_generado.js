import Ajv from "ajv";
import addFormats from "ajv-formats";

const ajv = new Ajv();
addFormats(ajv);

const schema = {
  "type": "object",
  "title": "Usuario",
  "properties": {
    "nombre": {
      "type": "string",
      "minLength": 3,
      "maxLength": 80
    },
    "edad": {
      "type": "number",
      "minimum": 18,
      "maximum": 99
    },
    "correo": {
      "type": "string",
      "format": "email"
    }
  },
  "required": ["nombre", "edad", "correo"]
};

export const validarUsuario = ajv.compile(schema);
